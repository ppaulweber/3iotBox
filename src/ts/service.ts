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

// import { sprintf } from "sprintf-js";

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
, { name:         'synchronized'
  , alias:        'S'
  , type:         Boolean
  , defaultValue: false
  }
, { name:         'configuration'
  , alias:        'c'
  , defaultValue: './config.json'
  }
, { name:         'delay'
  , alias:        'd'
  , type:         Number
  , defaultValue: 1000
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
 || typeof configuration.IOT.url === 'undefined'
 || typeof configuration.IOT.api === 'undefined'
 || typeof configuration.IOT.customerId === 'undefined'
 || typeof configuration.IOT.deviceSiteId === 'undefined'
 || typeof configuration.IOT.username === 'undefined'
 || typeof configuration.IOT.password === 'undefined'
 || typeof configuration.OFS === 'undefined'
 || typeof configuration.OFS.url === 'undefined'
 || typeof configuration.OFS.api === 'undefined'
 || typeof configuration.OFS.username === 'undefined'
 || typeof configuration.OFS.password === 'undefined'
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
// OFS
//

type OFS_Configuration = any;
type OFS_Callback = ( data : any ) => any;

function OFS_getReceiptAsJSON
( configuration: OFS_Configuration
, receiptId : string
, success : OFS_Callback = OFS_Callback => {}
, failure : OFS_Callback = OFS_Callback => {}
)
{
    let endpoint = '/belege/' + receiptId;
    let request = configuration.url + configuration.api + endpoint;
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
, success : OFS_Callback = OFS_Callback => {}
, failure : OFS_Callback = OFS_Callback => {}
)
{
    const qruri = require('@pgaubatz/qruri');
    const data = qruri( configuration.url + '/beleg/' + receiptId );    
    success( data );
}

function OFS_getAuth
( configuration: OFS_Configuration
, success : OFS_Callback = OFS_Callback => {}
, failure : OFS_Callback = OFS_Callback => {}
)
{
    const endpoint = '/auth';
    const request = configuration.url + configuration.api + endpoint;
    const accessToken = Buffer.from
    ( configuration.username + ':' + configuration.password ).toString( 'base64' );
    let communication = rest.get
    ( request
    , { headers:
        { 'Accept': 'application/json'
        , 'Authorization': `Basic ${accessToken}`
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

function OFS_getLatestReceipt
( configuration: OFS_Configuration
, accessToken : string
, cashRegisterId : string
, success : OFS_Callback = OFS_Callback => {}
, failure : OFS_Callback = OFS_Callback => {}
)
{
    const endpoint = `/registrierkassen/${cashRegisterId}/belege?format=beleg&order=desc&limit=1`;
    const request = configuration.url + configuration.api + endpoint;
    let communication = rest.get
    ( request
    , { headers:
        { 'Accept': 'application/json'
        , 'Authorization': `Bearer ${accessToken}`
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

function OFS_getReceipts
( configuration: OFS_Configuration
, accessToken : string
, cashRegisterId : string
, lastReceipt : any
, success : OFS_Callback = OFS_Callback => {}
, failure : OFS_Callback = OFS_Callback => {}
)
{
    const sync = Number(lastReceipt.Belegdaten.Belegnummer) + 1
    const endpoint = `/registrierkassen/${cashRegisterId}/belege?format=beleg&order=desc&gte=${sync}`;
    const request = configuration.url + configuration.api + endpoint;
    let communication = rest.get
    ( request
    , { headers:
        { 'Accept': 'application/json'
        , 'Authorization': `Bearer ${accessToken}`
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


//
//
// IOT
//

type IOT_Configuration = any;
type IOT_Callback = ( data : any ) => any;

function IOT_getConfig
( configuration: IOT_Configuration
, id : number
, success : IOT_Callback = IOT_Callback => {}
, failure : IOT_Callback = IOT_Callback => {}
)
{
    let endpoint
        = '/customers/' + configuration.customerId
        + '/sites/' + configuration.deviceSiteId
        + '/config' + id;
    let request = configuration.url + configuration.api + endpoint;
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
)
{
    let endpoint
        = '/customers/' + configuration.customerId
        + '/sites/' + configuration.deviceSiteId
        + '/config' + id;
    let request = configuration.url + configuration.api + endpoint;

    console.log( request );
    // console.log( data );
    
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
    if( !args[ 'synchronized' ] )
    {        
        setTimeout( processInput, args.delay );
    }
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

function processReceipt( receipt : any )
{
    let uuid = receipt.uuid;
    
    if( currentReceipt && currentReceipt.uuid == uuid )
    {
        // already updated the IoT receipt information
        processDelay();
        return;
    }

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

                      var data : number[][] = [];
                      for( var y= 0; y < 104; y++ )
                      {
                          data.push( [] );
                          for( var x = 0; x < 4; x++ )
                          {
                              data[ y ].push( 0 );
                          }
                      }

                      let pixelStream = pixels2Stream( pixels, 2, 96, 160, true );
                      // console.log( pixelStream );
                      
                      var x = 0;
                      var y = 0;
                      var pixel = 32;
                      for( let c of pixelStream )
                      {
                          if( c == '\n' )
                          {
                              // console.log( y );
                              y++;
                              x = 0;
                              pixel = 32;
                              continue;
                          }

                          if( c == 'b' && pixel != 32 )
                          {
                              // console.log( 'found black @ ' + x + ', ' + y );
                              data[ y ][ x ] = data[ y ][ x ] | (1 << (pixel-1));
                          }
                          
                          pixel--;
                          if( pixel == 0 )
                          {
                              x++;
                              pixel = 32;
                          }
                      }
                      
                      // var dbg = '';
                      // for( var y = 0; y < 104; y++ )
                      // {
                      //     for( var x = 0; x < 4; x++ )
                      //     {
                      //         let num = data[ y ][ x ];
                      //         dbg +=  sprintf( "%32s ", dec2Bin( num ) );
                      //     }
                      //     dbg += '\n';
                      // }
                      // console.log( dbg );
                      
                      let receiptObject : any =
                          { sync: receipt.stamp
                          , uuid: receipt.uuid
                          };

                      var index = 0;
                      for( var y = 0; y < 104; y++ )
                      {
                          for( var x = 0; x < 4; x++ )
                          {
                              let num = data[ y ][ x ];
                              receiptObject[ 'data' + index ] = num;
                              index++;
                          }
                      }
                      for( ; index <= 983; index++ )
                      {
                          receiptObject[ 'data' + index ] = 0;
                      }

                      IOT_setConfig
                      ( configuration.IOT
                      , 2
                      , receiptObject
                      , ( data : any ) =>
                        {                                
                            currentReceipt = receiptObject;
                            processDelay();
                            }
                      , IOT_error
                      );
                  }
                );
            }
            , OFS_error
          );
      }
      , OFS_error
    );
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
}


var syncAuth : any = null;
var syncReceipt : any = null;

function syncInput
(
)
{
    if( syncAuth == null )
    {
        OFS_getAuth
        ( configuration.OFS
        , ( data : any ) =>
          {
              syncAuth = data;
              syncInput();
          }
        , OFS_error
        );
        return;
    }

    if( syncReceipt == null )
    {
        OFS_getLatestReceipt
        ( configuration.OFS
        , syncAuth.accessToken
        , syncAuth.registrierkasseUuid
        , ( data : any ) =>
          {
              syncReceipt = data.Belege[0];
              syncInput();
          }
        , OFS_error
        );
        return;
    }

    OFS_getReceipts
    ( configuration.OFS
    , syncAuth.accessToken
    , syncAuth.registrierkasseUuid
    , syncReceipt
    , ( data : any ) =>
      {
          if( data.Belege.length > 0 )
          {
              syncReceipt = data.Belege[0];

              let receiptObject =
                  { uuid: syncReceipt._uuid
                  , url: configuration.OFS.url
                  , api: configuration.OFS.api
                  };
              
              IOT_setConfig
              ( configuration.IOT
              , 1
              , receiptObject
              , ( data : any ) =>
              {
                  processInput();
                  setTimeout( syncInput, args.delay );
              }
              , IOT_error
              );
          }
          else
          {
              setTimeout( syncInput, args.delay );
          }
      }
    , OFS_error
    );
}

// function dec2Bin
// ( dec : number
// )
// {
//     if( dec >= 0 )
//     {
//         return dec.toString( 2 );
//     }
//     else
//     {
//         return ( ~dec ).toString( 2 );
//     }
// }

if( args[ 'synchronized' ] )
{
    syncInput();
}
else
{
    processInput();
}

//
//  Local variables:
//  mode: javascript
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
