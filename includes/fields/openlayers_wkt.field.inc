<?php
  
 /**
  * Implements hook_fixtures_prepare_field
  * Prepare location fields
  *
  * @param $data string of location i.e. GEOMETRYCOLLECTION(POINT(1.0626685 52.2972388))
  * @return array
  * @author Rachel Graves
  **/
function fixtures_fixtures_field_openlayers_wkt_prepare($field_value, $field) {
  $prepared_field = array();
  $prepared_field[]['openlayers_wkt'] = $field_value;     
  
  return $prepared_field;  
}     


/**      
 * Implementes hook_fixtures_field_FIELD_TYPE_template
 *
 * @return a formatted example for a fixture's template
 * @todo take note of allowed_values, allowed_values_php,
 *  prefix, suffix, required, default_value, default_value_php
 */
function fixtures_fixtures_field_openlayers_wkt_template($field) {
    
  return array(
    'example' => 'GEOMETRYCOLLECTION(POINT(1.0626685 52.2972388))',
    'type' => $field['type'],
  );
  
}