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

var WebSocket = require('ws').Server;
var Args = require('command-line-args')

const argsDefinition =
[ { name:         'service-port'
  , alias:        's'
  , type:         Number
  , defaultValue: 8080
  }
];

var options = null;

try
{
    args = Args( argsDefinition );
}
catch( e )
{
    console.error( "3iotBox.js: " + e.message );
    process.exit();
}

// console.log( args );

var Express = require( 'express' );
const httpPort = args[ 'service-port' ];

var httpServer = Express();
httpServer.use( '/', Express.static( __dirname + '/../html' ) );
httpServer.use( '/_nodejs', Express.static( __dirname + '/../../obj/node' ) );
httpServer.use( '/_module', Express.static( __dirname + '/../../node_modules' ) );
httpServer.listen( httpPort );
console.log( '3iotBox.js: serving content on http://localhost:' + httpPort );

//
//  Local variables:
//  mode: javascript
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
