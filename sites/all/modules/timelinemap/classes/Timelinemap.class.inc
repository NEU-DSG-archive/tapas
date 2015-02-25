<?php

/**
 * @file
 * Class Timelinemap
 */

class Timelinemap
{

  var $view;
  var $data;
  var $sets;
  var $dateformat;
  var $color_id = 0;
  var $options;

  /**
   * Constructor.
   * @param $view
   */
  function __construct(&$style, $sets)
  {
    $this->options = $style->options;
    $this->view = $style->view;
    $this->sets = $sets;
    $this->dateformat = 'iso8601';

    $this->view->handlers = $style->display->handler->get_handlers('field');

    $this->data = $this->getEvents();
    $this->data['id'] = $style->view->name . '-' . $style->view->current_display;
    $this->data['focus'] = drupal_strtoupper($this->view->style_plugin->options['bands']['focus']);
    $this->data['band1_unit'] = drupal_strtoupper($this->view->style_plugin->options['bands']['band1_unit']);
    $this->data['band1_unit'] = drupal_strtoupper($this->view->style_plugin->options['bands']['band2_unit']);
    $this->data['datasource'] = $this->get_type();

    $this->data['pathcolor'] = $this->view->style_plugin->options['pathcolor'];
    $this->data['display_name'] = $style->view->name . '-' . $style->view->current_display;

    $counter = 0;
    /*embeding*/
    $this->data['view_link'] = 'admin/build/views/edit/' . $style->view->name . '#views-tab-' . $style->view->current_display;

    if (module_exists('vidi')) {
      $count = db_query("SELECT COUNT(*) FROM {vidi_visualization_tables} WHERE tablename = :tablename", array(':tablename' => $this->view->base_table))->fetchField();
      if ($count) {
        $storage['tablename'] = $this->view->base_table;
        $storage['datasource'] = $this->data['datasource'];
        $storage['visualization_title'] = ''; //$storage['visualization_title'] = $this->data['visualization_title'];
        $storage['width'] = $this->options['display']['width'];
        $storage['tlheight'] = $this->options['display']['tlheight'];
        $storage['mlheight'] = $this->options['display']['mlheight'];
        $storage['embed_link'] = $this->options['display']['embed_link'];
        $storage['title'] = $this->options['fields']['title'];
        $storage['start'] = $this->options['fields']['start'];
        $storage['end'] = $this->options['fields']['end'];
        $storage['table'] = $this->options['fields']['table'];
        $storage['latitude'] = $this->options['fields']['latitude'];
        $storage['longitude'] = $this->options['fields']['longitude'];
        $storage['pathcolor'] = $this->options['pathcolor'];
        $storage['colors'] = $this->options['colors']['values'];
        $storage['band1_unit'] = $this->options['bands']['band1_unit'];
        $storage['band2_unit'] = $this->options['bands']['band2_unit'];

        $this->data['storage'] = serialize($storage);
        $this->data['vidi_link'] = 'vidi/timelinemap_form/' . $this->view->base_table;
      }
      else {
        $this->data['storage'] = '';
        $this->data['vidi_link'] = '';
      }
    } else {
      $this->data['storage'] = '';
      $this->data['vidi_link'] = '';
    }


  }

  /*
  * Send data to json - Drupal.setting.timelinemap
  *
  */
  function timelinemap_to_json()
  {
    drupal_add_js(array('timelinemap' => array($this->data['id'] => $this->data)), 'setting');
  }

  /*
  * Make TimelinemapEvent objects for each $sets
  *
  */
  function getEvents()
  {
    $events = array();
    if (!empty($this->view->result)) {
      foreach ($this->sets as $k => $set) {
        //foreach ($this->view->style_plugin->rendered_fields as $k => $set) {
        $css_selector = $this->generateCssSelector();
        $color = FALSE;
        /*  if (isset($this->view->style_options['grouping']) && $this->view->style_options['grouping'] != '') { */
        $color = $this->getColor();
        /* } */

        foreach ($set as $key => $row) {
          $event = new TimelinemapEvent($this->view, $row, $key, $this->dateformat, $css_selector, $color);
          if ($event->getData()) {
            $events[] = $event->getData();
          }
        }
      }
    }
    return array(
      'events' => $events,
      'dateTimeFormat' => $this->dateformat,
    );
  }

  /*
  * Get timemap display type (basic or pathline)
  *
  */
  function get_type()
  {
    $type = $this->view->style_plugin->options['datasource'];
    return $type;
  }

  function getColor()
  {
    if (isset($this->view->style_plugin->options['colors']['values'])) {
      switch ($this->view->style_plugin->options['colors']['values']) {
        case 'city_funky':
          $colors_values = array('yellow', 'green', 'light blue', 'orange', 'purple');
          break;
        case 'pure_classic':
          $colors_values = array('blue');
          break;
        case 'ecology_green':
          $colors_values = array('green');
          break;
        case 'clear_blue':
          $colors_values = array('light blue');
          break;
        case 'lavender':
          $colors_values = array('purple');
          break;
        case 'burgundy_red':
          $colors_values = array('red');
          break;
        case 'orange_sun':
          $colors_values = array('orange');
          break;
        case 'vidi_special':
          $colors_values = array('yellow', 'green', 'light blue', 'orange');
      }

      $colors = array_values($colors_values);
      $tmp = $this->color_id;
      $this->color_id++;
      while ($tmp >= count($colors)) {
        $tmp = $tmp - count($colors);
      }
      return $colors[$tmp];
    }
    else return 'red';
  }

  function generateCssSelector()
  {
    return $this->view->name . '-set-' . $this->color_id;
  }
}