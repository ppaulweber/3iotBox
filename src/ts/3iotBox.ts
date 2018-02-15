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

if( typeof window === 'undefined' )
{
    console.log( 'script not running in `browser`' );
    process.exit();
}

import * as $ from 'jquery'


//window.onload = () =>
//{

const w = <any>window;
console.log( w );

$.support.cors = true;

$( "#3iotBox" ).append( '<div>3iotBox</div>' );
$( "#3iotBox" ).append( '<div id="3iotBox-png"></div>' );
$( "#3iotBox" ).append( '<div id="3iotBox-pdf"></div>' );
$( "#3iotBox" ).append( '<div id="3iotBox-json"></div>' );

$.ajax
( { url: 'https://demo.obono.at/api/v1/export/pdf/belege/NBjmYNKsPLahmXa5edjfje'
  , type: 'GET'
  , beforeSend: ( xhr : any ) =>
    { 
        xhr.setRequestHeader( 'Accept', 'application/pdf' );
    }
  , xhrFields:
    {
        responseType: 'blob'
    }
  }
).then
( ( data : any, status : any, jqxhr : any ) =>
  {
      var url = w.URL || w.webkitURL;
      var file = url.createObjectURL( data );
      console.log( file )
      $( "#3iotBox-pdf" ).append( '<embed src="' + file + '" type="application/pdf" />' );
  }
, ( jqxhr : any, status : any, error : any ) =>
  {
      console.log( jqxhr );
      console.log( status );
      console.log( error );
  }
);

//
//  Local variables:
//  mode: javascript
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
