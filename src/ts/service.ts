//
//  Copyright (C) 2018 Philipp Paulweber, Simon Tragatschnig, and Patrick Gaubatz
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                <https://github.com/ppaulweber/3iotBox>
//
//  This file is part of 3iotBox.
//
//  3iotBox is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  3iotBox is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with 3iotBox. If not, see <http://www.gnu.org/licenses/>.
//

if( typeof window !== 'undefined' )
{
    console.log( 'script not running in `node`' );
    process.exit();
}

let ARGS = require( 'command-line-args' );
let FS = require( 'fs' );
let EXPRESS = require( 'express' );
let REST = require( 'node-rest-client' ).Client;
let BASE58 = require( 'bs58' );
let UUID = require( '@pgaubatz/uuid' );
let GET_PIXELS = require( 'get-pixels' );
let WEBSHOT = require( 'webshot' );
let JIMP = require('jimp');

let argsDefinition =
[ { name:         'port'
  , alias:        'p'
  , type:         Number
  , defaultValue: 8080
  }
, { name:         'standalone'
  , alias:        's'
  , type:         Boolean
  , defaultValue: false
  }
, { name:         'configuration'
  , alias:        'c'
  , defaultValue: 'config.json'
  }
, { name:         'delay'
  , alias:        'd'
  , type:         Number
  , defaultValue: 500
  }
];

var args : any = null;

try
{
    args = ARGS( argsDefinition );
}
catch( e )
{
    console.error( __filename + ': ' + e.message );
    process.exit();
}

var configuration = JSON.parse( FS.readFileSync( args.configuration, 'utf8') );
if( typeof configuration.IOT === 'undefined'
 || typeof configuration.IOT.customerId === 'undefined'
 || typeof configuration.IOT.deviceSiteId === 'undefined'
 || typeof configuration.IOT.username === 'undefined'
 || typeof configuration.IOT.password === 'undefined'
 || typeof configuration.OFS === 'undefined'
  )
{
    console.log( 'invalid configuration file "' + args.configuration + '"' );
    process.exit();
}

if( args.standalone )
{
    let httpPort = args.port;

    var allowCrossDomain = ( req : any, res : any, next : any ) =>
    {
	    res.header( 'Access-Control-Allow-Origin', '*' );
	    res.header( 'Access-Control-Allow-Methods', 'DELETE, PUT, GET, POST' );
	    res.header( 'Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content-Type, Accept' );
	    next();
    };

    var httpServer = EXPRESS();
    httpServer.use( allowCrossDomain );
    httpServer.use( '/', EXPRESS.static( __dirname + '/../html' ) );
    httpServer.use( '/_nodejs', EXPRESS.static( __dirname + '/../../obj/src/ts' ) );
    httpServer.use( '/_module', EXPRESS.static( __dirname + '/../../node_modules' ) );
    httpServer.listen( httpPort );
    
    console.error( __filename + ': serving content on http://localhost:' + httpPort );
}


let rest = new REST();

//
//
// QRC
//

let QRC_URL = 'https://api.qrserver.com';
let QRC_API = '/v1';

type QRC_Callback = ( data : any ) => any;

function QRC_getPNG
( data : string
, size : number = 150
, success : QRC_Callback = QRC_Callback => {}
, failure : QRC_Callback = QRC_Callback => {}
, url : string = QRC_URL
, api : string = QRC_API
)
{
    let endpoint = '/create-qr-code/?size=' + size + 'x' + size + '&data=' + encodeURIComponent( data );
    let request = url + api + endpoint;
    var communication = rest.get
    ( request
    , { headers:
        { 'Content-Type': 'image/png'
        }
      }
    , ( data : any, response : any ) =>
      {
          // data is already PNG blob 
          console.log( __filename + ': ' + request + '\n' + 'PNG' );
          success( data );
      }
    );

    communication.on
    ( 'error'
    , ( error : any ) =>
      {
          let message = 'request error: ' + error;
          console.log( __filename + ': ' + message );
          failure( message );
      }
    );
}


//
//
// OFS
//

let OFS_URL = 'https://demo.obono.at'
let OFS_API = '/api/v1'

type OFS_Configuration = any;
type OFS_Callback = ( data : any ) => any;

function OFS_getReceiptAsJSON
( configuration: OFS_Configuration
, receiptId : string
, success : OFS_Callback = OFS_Callback => {}
, failure : OFS_Callback = OFS_Callback => {}
, url : string = OFS_URL
, api : string = OFS_API
)
{
    let endpoint = '/belege/' + receiptId;
    let request = url + api + endpoint;
    var communication = rest.get
    ( request
    , { headers:
        { 'Accept': 'application/json'
        }
      }
    , ( data : any, response : any ) =>
      {
          // data is JS object, parsed from JSON string
          console.log( __filename + ': ' + request + '\n' + JSON.stringify( data ) );

          if( data.status == "error" )
          {
              failure( data.message );
          }
          else
          {
              success( data );
          }
      }
    );

    communication.on
    ( 'error'
    , ( error : any ) =>
      {
          let message = 'request error: ' + error;
          console.log( __filename + ': ' + message );
          failure( message );
      }
    );
}

function OFS_getReceiptLinkAsQrCodePNG
( configuration: OFS_Configuration
, receiptId : string
, size : number
, success : OFS_Callback = OFS_Callback => {}
, failure : OFS_Callback = OFS_Callback => {}
, url : string = OFS_URL
, api : string = OFS_API
)
{
    QRC_getPNG
    ( 'https://demo.obono.at/beleg/' + receiptId
    , size
    , ( data : any ) =>
      {
          // data is blob, create PNG binary buffer
          let png = new Buffer( data, 'binary' );
          success( png );
      }
    , failure
    );
}

function OFS_getReceiptAsHTML
( configuration: OFS_Configuration
, receiptId : string
, success : OFS_Callback = OFS_Callback => {}
, failure : OFS_Callback = OFS_Callback => {}
, url : string = OFS_URL
, api : string = OFS_API
)
{
    let endpoint = '/export/html/belege/' + receiptId;
    let request = url + api + endpoint;
    var communication = rest.get
    ( request
    , { headers:
        { 'Accept': 'text/html'
        }
      }
    , ( data : any, response : any ) =>
      {
          // data is HTML text
          console.log( __filename + ': ' + request + '\n' + 'HTML' );
          success( data );
      }
    );

    communication.on
    ( 'error'
    , ( error : any ) =>
      {
          let message = 'request error: ' + error;
          console.log( __filename + ': ' + message );
          failure( message );
      }
    );
}


//
//
// IOT
//

let IOT_URL = 'https://iot.drei.at';
let IOT_API = '/api/1';

type IOT_Configuration = any;
type IOT_Callback = ( data : any ) => any;

function IOT_getConfig
( configuration: IOT_Configuration
, id : number
, success : IOT_Callback = IOT_Callback => {}
, failure : IOT_Callback = IOT_Callback => {}
, url : string = IOT_URL
, api : string = IOT_API
)
{
    let endpoint
        = '/customers/' + configuration.customerId
        + '/sites/' + configuration.deviceSiteId
        + '/config' + id;
    let request = url + api + endpoint;
    var communication = rest.get
    ( request
    , { headers:
        { 'Authorization': 'Basic ' + Buffer.from(configuration.username + ':' + configuration.password).toString('base64')
        , 'Content-Type': 'application/json'
        }
      }
    , ( data : any, response : any ) =>
      {
          console.log( __filename + ': ' + request + '\n' + data );

          // data is JSON string, parse to JS object
          let obj = JSON.parse( data ); // TODO: PPA: check for JSON parse errors

          if( obj.err )
          {
              failure( obj );
          }
          else
          {
              success( obj );
          }
      }
    );

    communication.on
    ( 'error'
    , ( error : any ) =>
      {
          console.log( __filename + ': request error: ' + error );
          failure( { err : error } );
      }
    );
}

function IOT_setConfig
( configuration: IOT_Configuration
, id : number
, data : object
, success : IOT_Callback = IOT_Callback => {}
, failure : IOT_Callback = IOT_Callback => {}
, url : string = IOT_URL
, api : string = IOT_API
)
{
    let endpoint
        = '/customers/' + configuration.customerId
        + '/sites/' + configuration.deviceSiteId
        + '/config' + id;
    let request = url + api + endpoint;
    var communication = rest.put
    ( request
    , { headers:
        { 'Authorization': 'Basic ' + Buffer.from(configuration.username + ':' + configuration.password).toString('base64')
        , 'Content-Type': 'application/json'
        }
      , data : data
      }
    , ( data : any, response : any ) =>
      {
          console.log( __filename + ': ' + request + '\n' + data );
          success( data );
      }
    );

    communication.on
    ( 'error'
    , ( error : any ) =>
      {
          console.log( __filename + ': request error: ' + error );
          failure( { err : error } );
      }
    );
}


function processDelay()
{
    setTimeout( processInput, args.delay );
}

function OFS_error( data : any )
{
    console.log( data );
    processDelay();
}

function IOT_error( data : any )
{
    console.log( data.err );
    processDelay();
}

let currentReceipt : any = null;

function processInput()
{
    if( !currentReceipt )
    {
        // service started, fetch first possible old receipt output
        IOT_getConfig
        ( configuration.IOT
        , 2
        , ( data : any ) =>
          {
              currentReceipt = data;
              processInput();
          }
        , IOT_error
        );
    }
    else
    {
        // fetched already info, process the IoT input data
        IOT_getConfig
        ( configuration.IOT
        , 1
        , processReceipt
        , IOT_error
        );
    }
}

var framebuffer : string[][] = null;

function processReceipt( receipt : any )
{
    let uuid = receipt.uuid;
    
    if( framebuffer && currentReceipt && currentReceipt.uuid == uuid )
    {
        // already updated the IoT receipt information
        processDelay();
        return;
    }

    framebufferClear();
    //framebufferDump();
    
    OFS_getReceiptAsJSON
    ( configuration.OFS
    , uuid
    , ( data : any ) =>
      {
          let receiptAsJSON = data;
          // uuid is a valid OFS receipt in the current configuration
          let uuid_b58 = BASE58.encode( UUID.parse( receiptAsJSON._uuid ) );
          if( uuid != uuid_b58 && uuid != receiptAsJSON._uuid )
          {
              let message = 'inconsistent UUID values found: ' + uuid + ' != ' + uuid_b58;
              console.log( __filename + ': ' + message );
              OFS_error( message );
              return;
          }

          OFS_getReceiptLinkAsQrCodePNG
          ( configuration.OFS
          , uuid
          , 60
          , ( png : any ) =>
            {
                GET_PIXELS
                ( png
                , 'image/png'
                , ( error : any, pixels : any ) =>
                  {
                      if( error )
                      {
                          OFS_error( 'png2pixels: ' + error );
                          return;
                      }
                      
                      /*let receiptAsQrCodeStream = */
                      pixels2Stream( pixels, 2, 96, 160, true );
                      pixels2Framebuffer( pixels );
                      
                      OFS_getReceiptAsHTML
                      ( configuration.OFS
                      , uuid
                      , ( html : any ) =>
                        {
                            var renderStream = WEBSHOT
                            ( html
                            , { siteType: 'html'
                              , screenSize:
                                { width: 322
                                }
                              , quality: 100
                              }
                            );

                            let fileName = 'obj/beleg.png';
                            var file = FS.createWriteStream( fileName, { encoding: 'binary' } );
                            
                            renderStream.on
                            ( 'data'
                            , ( data : any ) =>
                              {
                                  file.write( data.toString( 'binary' ), 'binary' );
                              }
                            );
                            
                            renderStream.on
                            ( 'end'
                            , () =>
                              {
                                  JIMP.read
                                  ( fileName
                                  , ( error : any, image : any ) =>
                                    {
                                        if( error )
                                        {
                                            OFS_error( error );
                                            return;
                                        }
                                        
                                        image.crop
                                        ( 20, 10, image.bitmap.width - 40, image.bitmap.height - 300
                                        ).posterize
                                        ( 4
                                        ).greyscale(
                                        ).write
                                        ( fileName + '.jimp.png'
                                        , () =>
                                          {
                                              GET_PIXELS
                                              ( fileName + '.jimp.png'
                                              , ( error : any, pixels : any ) =>
                                                {
                                                    if( error )
                                                    {
                                                        OFS_error( 'png2pixels: ' + error );
                                                        return;
                                                    }
                                              
                                                    //let receiptAsImageStream =
                                                    // pixels2Stream( pixels, 1, 64, 128 );
                                                    // console.log( receiptAsImageStream );
                                                    // pixels2Framebuffer( pixels, 0, 60 );

                                                    //framebufferDump();
                                                    var fb2s = framebuffer2Stream();
                                                    
                                                    let receiptObject =
                                                    { sync: receipt.stamp
                                                    , uuid: receipt.uuid
                                                    , data0 : fb2s[0]
                                                    , data1 : fb2s[1]
                                                    , data2 : fb2s[2]
                                                    , data3 : fb2s[3]
                                                    , data4 : fb2s[4]
                                                    , data5 : fb2s[5]
                                                    , data6 : fb2s[6]
                                                    , data7 : fb2s[7]
                                                    , data8 : fb2s[8]
                                                    , data9 : fb2s[9]
                                                    , data10 : fb2s[10]
                                                    , data11 : fb2s[11]
                                                    , data12 : fb2s[12]
                                                    , data13 : fb2s[13]
                                                    , data14 : fb2s[14]
                                                    , data15 : fb2s[15]
                                                    , data16 : fb2s[16]
                                                    , data17 : fb2s[17]
                                                    , data18 : fb2s[18]
                                                    , data19 : fb2s[19]
                                                    , data20 : fb2s[20]
                                                    , data21 : fb2s[21]
                                                    , data22 : fb2s[22]
                                                    , data23 : fb2s[23]
                                                    , data24 : fb2s[24]
                                                    , data25 : fb2s[25]
                                                    , data26 : fb2s[26]
                                                    , data27 : fb2s[27]
                                                    , data28 : fb2s[28]
                                                    , data29 : fb2s[29]
                                                    , data30 : fb2s[30]
                                                    , data31 : fb2s[31]
                                                    , data32 : fb2s[32]
                                                    , data33 : fb2s[33]
                                                    , data34 : fb2s[34]
                                                    , data35 : fb2s[35]
                                                    , data36 : fb2s[36]
                                                    , data37 : fb2s[37]
                                                    , data38 : fb2s[38]
                                                    , data39 : fb2s[39]
                                                    , data40 : fb2s[40]
                                                    , data41 : fb2s[41]
                                                    , data42 : fb2s[42]
                                                    , data43 : fb2s[43]
                                                    , data44 : fb2s[44]
                                                    , data45 : fb2s[45]
                                                    , data46 : fb2s[46]
                                                    , data47 : fb2s[47]
                                                    , data48 : fb2s[48]
                                                    , data49 : fb2s[49]
                                                    , data50 : fb2s[50]
                                                    , data51 : fb2s[51]
                                                    , data52 : fb2s[52]
                                                    , data53 : fb2s[53]
                                                    , data54 : fb2s[54]
                                                    , data55 : fb2s[55]
                                                    , data56 : fb2s[56]
                                                    , data57 : fb2s[57]
                                                    , data58 : fb2s[58]
                                                    , data59 : fb2s[59]
                                                    };
                                                    
                                                    IOT_setConfig
                                                    ( configuration.IOT
                                                    , 2
                                                    , receiptObject
                                                    , ( data : any ) =>
                                                      {
                                                          IOT_getConfig
                                                          ( configuration.IOT
                                                          , 2
                                                          );

                                                          currentReceipt = receiptObject;
                                                          processDelay();
                                                      }
                                                      , IOT_error
                                                    );
                                                }
                                              );
                                          }
                                        );
                                    }
                                  );
                              }
                            );
                        }
                      , OFS_error
                      );
                  }
                );
            }
            , OFS_error
          );
      }
      , OFS_error
      , receipt.url
      , receipt.api
    );
}

function pixels2Framebuffer
( pixels : any
, posX : number = 0
, posY : number = 0
, bg : number = 70
, gw : number = 140
)
{
    for( var x = 0; x < pixels.shape[0]; x += 1 )
    {
        for( var y = 0; y < pixels.shape[1]; y += 1 )
        {
            let pixel = ( pixels.get( y, x, 0 ) + pixels.get( y, x, 1 ) + pixels.get( y, x, 2 ) + pixels.get( y, x, 3 ) ) / 4;
            var framebufferPixel = '';
            
            if( pixel > gw )
            {
                framebufferPixel = 'w';
            }
            else if( pixel >= bg && pixel < gw )
            {
                framebufferPixel = 'g';
            }
            else
            {
                framebufferPixel = 'b';
            }

            if( ((posY + x) < 384) && ((posX + y) < 640) )
            {
                framebuffer[ posY + x ][ posX + y ] = framebufferPixel;
            }
        }
    }
}

function framebuffer2Stream
(
)
: string[]
{
    var pixelStream = [];
    for( var y = 0; y < 384; y++ )
    {
        pixelStream[ y ] = '';
        for( var x = 0; x < 640; x++ )
        {
            pixelStream[ y ] += framebuffer[ y ][ x ];
        }
    }
    return pixelStream;
}

function framebufferClear
(
)
{
    framebuffer = [];
    for( var y = 0; y < 384; y++ )
    {
        framebuffer[ y ] = [];
        for( var x = 0; x < 640; x++ )
        {
            framebuffer[ y ][ x ] = ' ';
        }
    }
}

function framebufferDump
(
)
{
    var fb2dbg = '';
    for( var y = 0; y < 384; y++ )
    {
        for( var x = 0; x < 640; x++ )
        {
            let fbp = framebuffer[ y ][ x ];
            if( fbp == 'w' )
            {
                fb2dbg += '\u001b[37;1m\u2588\u2588\u001b[0m'; // white as white
            }
            else if( fbp == 'g' )
            {
                fb2dbg += '\u001b[36;1m\u2588\u2588\u001b[0m'; // gray as cyan
            }
            else if( fbp == 'b' )
            {
                fb2dbg += '\u001b[34;1m\u2588\u2588\u001b[0m'; // blue as black
            }
            else
            {
                fb2dbg += '\u001b[34;1m\u2588\u2588\u001b[0m'; // none as yellow
            }
        }
        fb2dbg += '\n';
    }
    
    console.log( fb2dbg );
}

function pixels2Stream
( pixels : any
, increment : number = 1
, bg : number = 70
, gw : number = 140
, debug : boolean = false
)
: string
{
    var pixelStreamWidth = 0;
    var pixelStream = '';
    for( var x = 0; x < pixels.shape[0]; x += increment )
    {
        for( var y = 0; y < pixels.shape[1]; y += increment )
        {
            let pixel = ( pixels.get( y, x, 0 ) + pixels.get( y, x, 1 ) + pixels.get( y, x, 2 ) + pixels.get( y, x, 3 ) ) / 4;
            if( pixel > gw )
            {
                pixelStream += 'w';
            }
            else if( pixel >= bg && pixel < gw )
            {
                pixelStream += 'g';
            }
            else
            {
                pixelStream += 'b';
            }
        }
        pixelStream += 'w\n';
        pixelStreamWidth++;
    }
    if( increment > 1 )
    {
        for( var x = 0; x < pixels.shape[0]; x += increment )
        {
            pixelStream += 'w';
        }
        pixelStream += 'w';
        pixelStreamWidth++;
    }
    
    var pixelStreamDebug = '';
    for( let c of pixelStream )
    {
        if( c == 'w' )
        {
            pixelStreamDebug += '\u001b[37;1m\u2588\u2588\u001b[0m'; // white as white
        }
        else if( c == 'g' )
        {
            pixelStreamDebug += '\u001b[36;1m\u2588\u2588\u001b[0m'; // gray as cyan
        }
        else if( c == 'b' )
        {
            pixelStreamDebug += '\u001b[34;1m\u2588\u2588\u001b[0m'; // blue as black
        }
        else
        {
            pixelStreamDebug += c;
        }
    }

    if( debug )
    {
        console.log( pixelStreamDebug );
        console.log( pixelStreamWidth );
    }

    return pixelStream;

    // var pixelStreamCompressed = '' + pixelStreamWidth + 'x';
    // var pixelPrevious = '';
    // var pixelCounter = 0;
    // for( let c of pixelStream )
    // {
    //     if( c == '\n' )
    //     {
    //         continue;
    //     }
    //     if( c != pixelPrevious )
    //     {
    //         if( pixelCounter > 0 )
    //         {
    //             pixelStreamCompressed += pixelCounter;
    //             pixelStreamCompressed += pixelPrevious;
    //         }
    //         pixelCounter = 1;
    //         pixelPrevious = c;
    //         continue;
    //     }
    //     pixelCounter++;
    // }
    
    // return pixelStreamCompressed;
}

processInput();

//
//  Local variables:
//  mode: javascript
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
