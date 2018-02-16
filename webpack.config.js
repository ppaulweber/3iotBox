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

const path = require( 'path' );
const WebpackCopyPlugin = require( 'copy-webpack-plugin' );
const WebpackUglifyJsPlugin = require('webpack-uglify-js-plugin');
const buildRoot = path.resolve( __dirname, 'obj', 'src', 'ts' );

module.exports =
{ entry:
  { '3iotBox.min' : path.resolve( buildRoot, 'client.js' )
  , '3iotBox.dev' : path.resolve( buildRoot, 'client.js' )
  }
, output:
  { path: buildRoot
  , filename: '[name].js'
  }
, devtool: 'source-map'
, target: 'web'
, node:
  { fs: 'empty'
  , child_process: 'empty'
  , net: 'empty'
  , tls: 'empty'
  }
, plugins:
  []
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
