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

let UTIL = require( 'util' );
let GETPIXELS = require( 'get-pixels' );
// var JIMP = require("jimp");


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
];

var args = null;

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
    httpServer.use( '/_nodejs', EXPRESS.static( __dirname + '/../../obj/node' ) );
    httpServer.use( '/_module', EXPRESS.static( __dirname + '/../../node_modules' ) );
    httpServer.listen( httpPort );
    
    console.error( __filename + ': serving content on http://localhost:' + httpPort );
}


let rest = new REST();

//
//
// OFS
//

let OFS_URL = 'https://demo.obono.at'
let OFS_API = '/api/v1'

type OFS_Configuration = any;
type OFS_Callback = ( data : any ) => any;

function OFS_getReceipt
( configuration: OFS_Configuration
, receiptId : string
, success : OFS_Callback = OFS_Callback => {}
, failed : OFS_Callback = OFS_Callback => {}
, url : string = OFS_URL
, api : string = OFS_API
)
{
    let endpoint = '/belege/' + receiptId;
    let request = url + api + endpoint;
    var communication = rest.get
    ( request
    , { headers:
        { "Accept": "application/json"
        }
      }
    , ( data : any, response : any ) =>
      {
          // data is already object
          console.log( __filename + ": " + request + "\n" + JSON.stringify( data ) );
          success( data );
      }
    );

    communication.on
    ( 'error'
    , ( error : any ) =>
      {
          console.log( __filename + ": request error: " + error );
          failed( { err : error } );
      }
    );
}

// OFS get receipt PDF 
// curl -X GET --header 'Accept: application/pdf' 'https://demo.obono.at/api/v1/export/pdf/belege/UUID'


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
, failed : IOT_Callback = IOT_Callback => {}
, url : string = IOT_URL
, api : string = IOT_API
)
{
    let endpoint = '/customers/' + configuration.customerId + '/sites/' + configuration.deviceSiteId + '/config' + id;
    let request = url + api + endpoint;
    var communication = rest.get
    ( request
    , { headers:
        { "Authorization": "Basic " + Buffer.from(configuration.username + ":" + configuration.password).toString('base64')
        , "Content-Type": "application/json"
        }
      }
    , ( data : any, response : any ) =>
      {
          console.log( __filename + ": " + request + "\n" + data );
          
          // data is JSON string, parse to object
          let obj = JSON.parse( data ); // TODO: PPA: check for JSON parse errors

          if( obj.err )
          {
              failed( obj );
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
          console.log( __filename + ": request error: " + error );
          failed( { err : error } );
      }
    );
}

//
//
// QRC
//

let QRC_URL = 'https://api.qrserver.com';
let QRC_API = '/v1';

type QRC_Configuration = any;
type QRC_Callback = ( data : any ) => any;

function QRC_getPNG
( configuration: QRC_Configuration
, data : string
, size : number = 150
, success : QRC_Callback = QRC_Callback => {}
, failed : QRC_Callback = QRC_Callback => {}
, url : string = QRC_URL
, api : string = QRC_API
)
{
    let endpoint = '/create-qr-code/?size=' + size + 'x' + size + '&data=' + encodeURIComponent( data );
    let request = url + api + endpoint;
    var communication = rest.get
    ( request
    , { headers:
        { "Content-Type": "image/png"
        }
      }
    , ( data : any, response : any ) =>
      {
          // data is already PNG blob 
          console.log( __filename + ": " + request + "\n" + "PNG" );
          success( data );
      }
    );

    communication.on
    ( 'error'
    , ( error : any ) =>
      {
          console.log( __filename + ": request error: " + error );
          failed( { err : error } );
      }
    );
}


function kernelStep()
{
    setTimeout( kernelTask, 1000 );
}

function OFS_error( data : any )
{
    console.log( data );
    kernelStep();
}

function IOT_error( data : any )
{
    console.log( data.err );
    kernelStep();
}

function QRC_error( data : any )
{
    console.log( data.err );
    kernelStep();
}

var uptime = 0;
function kernelTask()
{
    console.log( __filename + ': uptime: ' + uptime );
    uptime++;

    IOT_getConfig
    ( configuration.IOT
    , 0
    , getClientInput
    , IOT_error
    );
}


function getClientInput()
{
    IOT_getConfig
    ( configuration.IOT
    , 1
    , ( data : any ) => getReceipt( data.uid, data.url )
    , IOT_error
    );    
}

function getReceipt( uuid : string, url : string )
{
    OFS_getReceipt
    ( configuration.OFS
    , uuid
    , ( data : any ) =>
      {
          // uuid is a valid OFS receipt in the current configuration
          let uuid_b58 = BASE58.encode( UUID.parse( data._uuid ) );
          if( uuid != uuid_b58 && uuid != data._uuid )
          {
              let message = 'inconsistent UUID values found: ' + uuid + ' != ' + uuid_b58;
              console.log( __filename + ': ' + message );
              OFS_error( { err : message } );
              return;
          }

          // TODO: write JSON data to IOT server
          getQrCodePNG( 'https://demo.obono.at/beleg/' + uuid );
      }
      , OFS_error
      , url
    );
}

function getQrCodePNG( data : string )
{
    QRC_getPNG
    ( configuration.QRC
    , data
    , 60
    , ( data : any ) =>
      {
          let pngBuffer = new Buffer( data, 'binary' );

          GETPIXELS
          ( pngBuffer, 'image/png'
          , ( err : any, pixels : any ) =>
            {
                if( err )
                {
                    QRC_error( { err : 'unable to parse PNG blob data' } );
                    return;
                }
                var qrCodeStream = "";
                for( var x = 0; x < pixels.shape[0]; x += 2 )
                {
                    for( var y = 0; y < pixels.shape[1]; y += 2 )
                    {
                        let pixel = pixels.get( y, x, 2 );
                        if( pixel > 128 )
                        {
                            qrCodeStream += '0';
                        }
                        else
                        {
                            qrCodeStream += '1';
                        }
                    }
                    qrCodeStream += '0\n';
                }
                for( var x = 0; x < pixels.shape[0]; x+=2 )
                {
                    qrCodeStream += '0';
                }
                qrCodeStream += '0';
                qrCodeStream = qrCodeStream.length + '\n' + qrCodeStream;
                
                var qrCodeStreamDebug = "";
                var first = true;
                for( let c of qrCodeStream )
                {
                    if( first )
                    {
                        if( c == '\n' )
                        {
                            first = false;
                        }
                        continue;
                    }
                    
                    if( c == '0' )
                    {
                        qrCodeStreamDebug += '\u2588\u2588';
                    }
                    else if( c == '1' )
                    {
                        qrCodeStreamDebug += '  ';
                    }
                    else
                    {
                        qrCodeStreamDebug += c;
                    }
                }
                
                console.log( qrCodeStreamDebug );
                // console.log( qrCodeStream );
            }
          );
          
          // JIMP.read( buffer, function( err, image ) {
          // });
      }
    , QRC_error
    );
}
    
kernelStep();

//
//  Local variables:
//  mode: javascript
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
