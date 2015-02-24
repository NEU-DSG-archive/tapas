<?php
/* double-commented to avoid conflict with svn
 */

/**
 * @file
 *  UI and forms for administering taxonomy_xml
 *
 * 2010  D7 Dan Morrison http://coders.co.nz
 *
 */


/**
 * Builds the import form.
 *
 * Form contains a selector to choose the import method used (upload, URL, Web
 * Service). This selector should reveal or hide the appropriate secondary
 * parameters. Uses JS and a bit of CSS to show/hide. With no JS, all options
 * are shown but only the chosen one is used.
 *
 * @see taxonomy_xml_import_form_submit()
 */
function taxonomy_xml_import_form($form, &$form_state) {
  $formats = taxonomy_xml_formats();
  $vocs[TAXONOMY_XML_DETERMINED_BY_SOURCE_FILE] = t('[Determined by source file]');
  foreach (taxonomy_get_vocabularies() as $vid => $voc) {
    $vocs[$vid] = $voc->name;
  }
  $vocs[TAXONOMY_XML_CREATE_NEW] = t('[Create new]');
  $form['vid'] = array(
    '#type' => 'select',
    '#title' => t('Target vocabulary'),
    '#default_value' => variable_get('taxonomy_xml_vid', TAXONOMY_XML_CREATE_NEW),
    '#options' => $vocs,
    '#description' => t('
      The vocabulary into which terms should be loaded.
      If you choose a pre-existing vocabulary,
      existing vocabulary settings (tags, node types etc) will NOT be modified.
      If it is to be created new, they <em>may</em> be retained.
      Internal vocabulary ID "vid" cannot be imported.
    '),
  );

  // This form allows the input from different types of source,
  // and different formats.
  // Each importer service may be loaded from external modules.
  // Loading this list also ensures the includes are present
  $services_info = taxonomy_xml_services();
  $services_list = taxonomy_xml_services('options');

  // The form has some UI additions to help select the input
  drupal_add_js(drupal_get_path('module', 'taxonomy_xml') . '/filtering_fieldset.js');

  $form['data_source'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('id' => 'data_source', 'class' => array('filtering-fieldset')),
    '#title' => t('Data Source'),
  );
  $form['data_source']['service_id'] = array(
    '#type' => 'select',
    #'#title' => t('Data Source'),
    '#options' => $services_list,
    '#id' => 'service_id',
    '#default_value' => variable_get('taxonomy_xml_service_id', 'none'),
    '#attributes' => array('class' => array('filtering-selector') ),
  );

  // Build a subform for each of the services
  // They will ask their own questions and take processing and submission from here.
  foreach ($services_info as $service_id => $service_info) {
    $service_data = @$form_state['values'][$service_id];
    // Individual services may insert their own buttons and UI here
    if (isset($service_info['import_form_callback'])) {
      $service_specific_form_callback = $service_info['import_form_callback'];
      $form['data_source'][$service_id] = $service_specific_form_callback($service_data, $service_info);
      // Decorate the returned sub-form, saves each service having to do so.
      $form['data_source'][$service_id]['#type'] = 'fieldset';
      $form['data_source'][$service_id]['#title'] = $service_info['name'];
      $form['data_source'][$service_id]['#description'] = $service_info['description'];
      $form['data_source'][$service_id]['#attributes']['class'][] = 'filtered-fieldset';
      $form['data_source'][$service_id]['#attributes']['class'][] = 'filtered-fieldset-' . $service_id;
      $form['data_source'][$service_id]['#tree'] = TRUE; // Each subset from here is in its own data array

      // If it hasn't provided a submit button, maybe add a default one
      if (! isset($form['data_source'][$service_id]['submit'])) {
        $submit_func = $service_specific_form_callback . '_submit';
        if (function_exists($submit_func)) {
          $form['data_source'][$service_id]['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Import from '. $service_info['name']),
            '#submit' => array($submit_func),
          );
        }
      }
    }
  }

  $form['recurse_down'] = array(
    '#type' => 'checkbox',
    '#title' => t('Recurse down the taxonomy tree'),
    '#description' => t('
      Some taxonomy sources return references to further external URL
      sources (child terms).
      Tick this if those references are to be followed.
      <br/>The recursion may get intensive, although the tasks will be "batched".
      <br/>Note: You will <b>need</b> taxonomy_enhancer or something similar to be
      recording the external IDs or relationships cannot be maintained
      over batches.
    '),
    '#default_value' => variable_get('taxonomy_xml_recurse_down', TRUE),
  );


  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['advanced']['duplicate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow duplicate terms'),
    '#description' => t('If you want to keep the same term in different positions in the vocabulary hierarchy, check this'),
    '#default_value' => variable_get('taxonomy_xml_duplicate', FALSE),
  );
  $form['advanced']['taxonomy_xml_watchdog_level'] = array(
    '#type' => 'select',
    '#title' => t('Debug Level'),
    '#description' => t('To assist development, taxonomy_xml has the ability to display parsing messages as they are logged.'),
    '#options' => taxonomy_xml_watchdog_levels(),
    '#default_value' => variable_get('taxonomy_xml_watchdog_level', WATCHDOG_NOTICE),
  );
  $form['advanced']['flush_cache_description'] = array(
    '#type' => 'markup', '#prefix' => '<p>', '#suffix' => '</p>',
    '#value' => t('When retrieving remote data, a local cache is kept of successful file downloads. These are not expected to change, but may get in the way during testing. Flush the cache to delete them <em>all</em>.'),
  );
  $form['advanced']['flush_cache'] = array(
    '#type' => 'submit',
    '#value' => t('Flush Cache'),
    '#submit' => array('taxonomy_xml_flush_file_cache'),
  );

  $form['#attributes'] = array('enctype' => 'multipart/form-data');

  return $form;
}

/**
 * Overall form persistence, should fire on any submission whatsoever.
 */
function taxonomy_xml_import_form_validate($form, &$form_state) {
  $form_state['storage']['remember'] = TRUE;
  // Do not redirect and leave the page after uploading a file. This keeps
  // all the current form values in place. The file is saved by the
  // #value_callback on the form element.
  $form_state['redirect'] = FALSE;
}

/**
 * Imports the actual XML.
 */
function taxonomy_xml_import_form_submit($form, &$form_state) {
  #if ( function_exists('xdebug_start_trace')) { xdebug_start_trace(); }

  module_load_include('inc', 'taxonomy_xml', 'taxonomy_xml.process');

  // Remember current prefs, just for convenience
  // Speaking of convenience, why does my form discard these values all the time?
  $service_id = $form_state['values']['service_id'];
  variable_set('taxonomy_xml_service_id', $service_id);
  variable_set('taxonomy_xml_format', @$form_state['values'][$service_id]['format']);
  variable_set('taxonomy_xml_identifier', @$form_state['values'][$service_id]['identifier']);
  variable_set('taxonomy_xml_url', @$form_state['values'][$service_id]['url']);
  variable_set('taxonomy_xml_vid', $form_state['values']['vid']);
  variable_set('taxonomy_xml_duplicate', $form_state['values']['duplicate']);
  variable_set('taxonomy_xml_recurse_down', $form_state['values']['recurse_down']);
  variable_set('taxonomy_xml_watchdog_level', $form_state['values']['taxonomy_xml_watchdog_level']);

  // Each of the respective services will take care of this form submission in their own way.

  // The initial import may have queued up some further process to do.
  // Check the queue and run it when this form goes.
  batch_set(taxonomy_xml_add_term_to_batch_queue());
}


