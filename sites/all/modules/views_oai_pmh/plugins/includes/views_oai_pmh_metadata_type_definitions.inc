<?php
/**
 * @file
 * OAI metadata type definitions.
 */

/**
 * An emergency default metadata format for the OAI output, e.g. 'oai_dc'. This
 * will be the type shown in the Views preview when configuring the view.
 * 
 * @global string $GLOBALS['views_oai_pmh_default']
 * @name $views_oai_pmh_default 
 */
$GLOBALS['views_oai_pmh_default'] = 'oai_dc';

/**
 * Definition of the OAI Dublin Core (DC) data format. 
 */
$oai_dc = new views_oai_pmh_metadata_type('oai_dc');
$oai_dc->name = t('Dublin Core');
$oai_dc->style_plugin = 'views_oai_pmh_dc';
$oai_dc->row_plugin = 'dc';
$oai_dc->form_group_name = 'oai_dc_fields';
$oai_dc->help = t('Displays the fields as Dublin Core elements.');
$oai_dc->base_xml_node = new views_oai_pmh_xml_node(
  'oai_dc:dc',
  NULL,
  array(
    'xmlns:xsi' => 'http://www.w3.org/2001/XMLSchema-instance',
    'xsi:schemaLocation' => 'http://www.openarchives.org/OAI/2.0/oai_dc/       http://www.openarchives.org/OAI/2.0/oai_dc.xsd',
    'xmlns:oai_dc' => 'http://www.openarchives.org/OAI/2.0/oai_dc/',
    'xmlns:dc' => 'http://purl.org/dc/elements/1.1/',
  )
);
$oai_dc->elements = array(
  'none'            => t('None'),
  'dc:title'       => 'dc:title',
  'dc:creator'     => 'dc:creator',
  'dc:subject'     => 'dc:subject',
  'dc:description' => 'dc:description',
  'dc:publisher'   => 'dc:publisher',
  'dc:contributor' => 'dc:contributor',
  'dc:date'        => 'dc:date',
  'dc:type'        => 'dc:type',
  'dc:format'      => 'dc:format',
  'dc:identifier'  => 'dc:identifier',
  'dc:source'      => 'dc:source',
  'dc:language'    => 'dc:language',
  'dc:relation'    => 'dc:relation',
  'dc:coverage'    => 'dc:coverage',
  'dc:rights'      => 'dc:rights',
);

// Assign the data format to the global array of OAI data formats.
$GLOBALS['views_oai_pmh']['oai_dc'] = $oai_dc;

/**
 * Definition of the OAI Learning Objects Metadata (LOM) data format. 
 */
$oai_lom = new views_oai_pmh_metadata_type('oai_lom');
$oai_lom->name = t('Learning Objects Metadata (LOM)');
$oai_lom->style_plugin = 'views_oai_pmh_lom';
$oai_lom->row_plugin = 'oai_lom';
$oai_lom->form_group_name = 'oai_lom_fields';
$oai_lom->help = t('Displays the fields as Learning Objects Metadata elements.');
$oai_lom->base_xml_node = new views_oai_pmh_xml_node(
  'oai_lom:lom',
  NULL,
  array(
    'xmlns:xsi' => 'http://www.w3.org/2001/XMLSchema-instance',
    'xsi:schemaLocation' => 'http://ltsc.ieee.org/xsd/LOM       http://ltsc.ieee.org/xsd/lomv1.0/lom.xsd',
    'xmlns:oai_lom' => 'http://ltsc.ieee.org/xsd/LOM',
    'xmlns:lom' => 'http://ltsc.ieee.org/xsd/LOM',
  )
);
$oai_lom->elements = array(
  'none' => t('None'),
    
  // misc elements
  '(attribute)oai_lom@language' => t('(Attribute) Language'),

  // general
  'oai_lom:general' => 'General (Requires Template)',
  'oai_lom:general/oai_lom:identifier/oai_lom:catalog' => 'General > Identifier > Catalog',
  'oai_lom:general/oai_lom:identifier/oai_lom:entry' => 'General > Identifier > Entry',
  'oai_lom:general/oai_lom:title/oai_lom:string@language' => 'General > Title',
  'oai_lom:general/oai_lom:language' => 'General > Language',
  'oai_lom:general/oai_lom:description/oai_lom:string@language' => 'General > Description',
  'oai_lom:general/oai_lom:keyword' => 'General > Keyword (Requires Template)',
  'oai_lom:general/oai_lom:keyword/oai_lom:string@language' => 'General > Keyword > String',
  'oai_lom:general/oai_lom:structure/oai_lom:source' => 'General > Structure > Source',
  'oai_lom:general/oai_lom:structure/oai_lom:value' => 'General > Structure > Value',
  'oai_lom:general/oai_lom:aggregationLevel/oai_lom:source' => 'General > Aggregation Level > Source',
  'oai_lom:general/oai_lom:aggregationLevel/oai_lom:value' => 'General > Aggregation Level > Value',

  // lifeCycle
  'oai_lom:lifeCycle' => 'Life Cycle (Requires Template)',
  'oai_lom:lifeCycle/oai_lom:version/oai_lom:string' => 'Life Cycle > Version',
  'oai_lom:lifeCycle/oai_lom:status/oai_lom:source' => 'Life Cycle > Status > Source',
  'oai_lom:lifeCycle/oai_lom:status/oai_lom:value' => 'Life Cycle > Status > Value',
  'oai_lom:lifeCycle/oai_lom:contribute/oai_lom:role/oai_lom:source' => 'Life Cycle > Contribute > Role > Source',
  'oai_lom:lifeCycle/oai_lom:contribute/oai_lom:role/oai_lom:value' => 'Life Cycle > Contribute > Role > Value',
  'oai_lom:lifeCycle/oai_lom:contribute/oai_lom:entity' => 'Life Cycle > Contribute > Entity',

  // metaMetadata
  'oai_lom:metaMetadata' => 'Meta Metadata (Requires Template)',
  'oai_lom:metaMetadata/oai_lom:identifier/oai_lom:catalog' => 'Meta Metadata > Identifier > Catalog',
  'oai_lom:metaMetadata/oai_lom:identifier/oai_lom:entry' => 'Meta Metadata > Identifier > Entry',
  'oai_lom:metaMetadata/oai_lom:contribute/oai_lom:role/oai_lom:source' => 'Meta Metadata > Contribute > Role > Source',
  'oai_lom:metaMetadata/oai_lom:contribute/oai_lom:role/oai_lom:value' => 'Meta Metadata > Contribute > Role > Value',
  'oai_lom:metaMetadata/oai_lom:contribute/oai_lom:entity' => 'Meta Metadata > Contribute > Entity',
  'oai_lom:metaMetadata/oai_lom:metadataSchema' => 'Meta Metadata > Metadata Schema',
  'oai_lom:metaMetadata/oai_lom:language' => 'Meta Metadata > Language',

  // technical
  'oai_lom:technical' => 'Technical (Requires Template)',
  'oai_lom:technical/oai_lom:format' => 'Technical > Format',
  'oai_lom:technical/oai_lom:location' => 'Technical > Location',

  // educational
  'oai_lom:educational' => 'Educational (Requires Template)',
  'oai_lom:educational/oai_lom:interactivityType' => 'Educational > Interactivity (Requires Template)',
  'oai_lom:educational/oai_lom:interactivityType/oai_lom:source' => 'Educational > Interactivity > Source',
  'oai_lom:educational/oai_lom:interactivityType/oai_lom:value' => 'Educational > Interactivity > Value',
  'oai_lom:educational/oai_lom:learningResourceType' => 'Educational > Learning Resource Type (Requires Template)',
  'oai_lom:educational/oai_lom:learningResourceType/oai_lom:source' => 'Educational > Learning Resource Type > Source',
  'oai_lom:educational/oai_lom:learningResourceType/oai_lom:value' => 'Educational > Learning Resource Type > Value',
  'oai_lom:educational/oai_lom:interactivityLevel' => 'Educational > Interactivity Level (Requires Template)',
  'oai_lom:educational/oai_lom:interactivityLevel/oai_lom:source' => 'Educational > Interactivity Level > Source',
  'oai_lom:educational/oai_lom:interactivityLevel/oai_lom:value' => 'Educational > Interactivity Level > Value',
  'oai_lom:educational/oai_lom:intendedEndUserRole' => 'Educational > Intended End User Role (Requires Template)',
  'oai_lom:educational/oai_lom:intendedEndUserRole/oai_lom:source' => 'Educational > Intended End User Role > Source',
  'oai_lom:educational/oai_lom:intendedEndUserRole/oai_lom:value' => 'Educational > Intended End User Role > Value',
  'oai_lom:educational/oai_lom:context' => 'Educational > Context (Requires Template)',
  'oai_lom:educational/oai_lom:context/oai_lom:source' => 'Educational > Context > Source',
  'oai_lom:educational/oai_lom:context/oai_lom:value' => 'Educational > Context > Value',
  'oai_lom:educational/oai_lom:typicalAgeRange/oai_lom:string@language' => 'Educational > Typical Age Range',
  'oai_lom:educational/oai_lom:difficulty' => 'Educational > Difficulty (Requires Template)',
  'oai_lom:educational/oai_lom:difficulty/oai_lom:source' => 'Educational > Difficulty > Source',
  'oai_lom:educational/oai_lom:difficulty/oai_lom:value' => 'Educational > Difficulty > Value',
  'oai_lom:educational/oai_lom:typicalLearningTime/oai_lom:duration' => 'Educational > Typical Learning Time > Duration',
  'oai_lom:educational/oai_lom:description/oai_lom:string@language' => 'Educational > Description',
  'oai_lom:educational/oai_lom:language' => 'Educational > Educational > Language',

  // rights
  'oai_lom:rights' => 'Rights (Requires Template)',
  'oai_lom:rights/oai_lom:cost/oai_lom:source' => 'Rights > Cost > Source',
  'oai_lom:rights/oai_lom:cost/oai_lom:value' => 'Rights > Cost > Value',
  'oai_lom:rights/oai_lom:copyrightAndOtherRestrictions/oai_lom:source' => 'Rights > Copyright (+ Other) > Source',
  'oai_lom:rights/oai_lom:copyrightAndOtherRestrictions/oai_lom:value' => 'Rights > Copyright (+ Other) > Value',
  'oai_lom:rights/oai_lom:description/oai_lom:string@language' => 'Rights > Description',

  // relation
  'oai_lom:relation' => 'Relation (Requires Template)',
  'oai_lom:relation/oai_lom:kind/oai_lom:source' => 'Relation > Kind > Source',
  'oai_lom:relation/oai_lom:kind/oai_lom:value' => 'Relation > Kind > Value',
  'oai_lom:relation/oai_lom:resource/oai_lom:description/oai_lom:string@language' => 'Relation > Resource > Description',
  'oai_lom:relation/oai_lom:resource/oai_lom:identifier/oai_lom:catalog' => 'Relation > Resource > Identifier > Catalog',
  'oai_lom:relation/oai_lom:resource/oai_lom:identifier/oai_lom:entry' => 'Relation > Resource > Identifier > Entry',

  // classification
  'oai_lom:classification' => 'Classification (Requires Template)',
  'oai_lom:classification/oai_lom:purpose/oai_lom:source' => 'Classification > Purpose > Source',
  'oai_lom:classification/oai_lom:purpose/oai_lom:value' => 'Classification > Purpose > Value',
  'oai_lom:classification/oai_lom:keyword/oai_lom:string@language' => 'Classification > Keyword',
);

// Assign the data format to the global array of OAI data formats.
$GLOBALS['views_oai_pmh']['oai_lom'] = $oai_lom;

/**
 * Definition of the OAI Learning Resource Exchange (LRE) data format. 
 */
$oai_lre = new views_oai_pmh_metadata_type('oai_lre');
$oai_lre->name = t('Learning Resource Exchange (LRE)');
$oai_lre->style_plugin = 'views_oai_pmh_lre';
$oai_lre->row_plugin = 'oai_lre';
$oai_lre->form_group_name = 'oai_lre_fields';
$oai_lre->help = t('Displays the fields as Learning Resource Exchange elements.');
$oai_lre->base_xml_node = new views_oai_pmh_xml_node(
  'expression',
  NULL,
  array(
    'xmlns:xsi' => 'http://www.w3.org/2001/XMLSchema-instance',
    'xsi:schemaLocation' => 'http://www.imsglobal.org/xsd/imslorsltitm_v1p0       http://fire.eun.org/lode/imslorsltitm_v1p0.xsd',
    'xmlns' => 'http://www.imsglobal.org/xsd/imslorsltitm_v1p0',
  )
);
$oai_lre->elements = array(
  'none' => t('None'),
  
  // misc elements
  '(attribute)oai_lre@language' => t('(Attribute) Language'),
  '(attribute)oai_lre@xsi:schemaLocation' => t('(Attribute) LOM XSI Schema Location'),
  '(attribute)oai_lre@xmlns' => t('(Attribute) LOM XML Namespace'),
  
  // identifier
  'identifier/catalog' => 'Identifier > Catalog',
  'identifier/entry' => 'Identifier > Entry',
  
  // description
  'description/facet/vocabularyID' => 'Description > Facet > Vocabulary ID',
  'description/facet/value' => 'Description > Facet > Value',
  
  // description -> LOM -> general
  'description/metadata/schema' => 'Description > Metadata > Schema',
  'description/metadata/lom@xsi:schemaLocation@xmlns/general' => 'Description > Metadata > LOM > General (Requires Template)',
  'description/metadata/lom@xsi:schemaLocation@xmlns/general/identifier/catalog' => 'Description > Metadata > LOM > General > Identifier > Catalog',
  'description/metadata/lom@xsi:schemaLocation@xmlns/general/identifier/entry' => 'Description > Metadata > LOM > General > Identifier > Entry',
  'description/metadata/lom@xsi:schemaLocation@xmlns/general/title/string@language' => 'Description > Metadata > LOM > General > Title',
  'description/metadata/lom@xsi:schemaLocation@xmlns/general/language' => 'Description > Metadata > LOM > General > Language',
  'description/metadata/lom@xsi:schemaLocation@xmlns/general/description/string@language' => 'Description > Metadata > LOM > General > Description',
  'description/metadata/lom@xsi:schemaLocation@xmlns/general/keyword' => 'Description > Metadata > LOM > General > Keyword (Requires Template)',
  'description/metadata/lom@xsi:schemaLocation@xmlns/general/keyword/string@language' => 'Description > Metadata > LOM > General > Keyword > String',
  'description/metadata/lom@xsi:schemaLocation@xmlns/general/structure/source' => 'Description > Metadata > LOM > General > Structure > Source',
  'description/metadata/lom@xsi:schemaLocation@xmlns/general/structure/value' => 'Description > Metadata > LOM > General > Structure > Value',
  'description/metadata/lom@xsi:schemaLocation@xmlns/general/aggregationLevel/source' => 'Description > Metadata > LOM > General > Aggregation Level > Source',
  'description/metadata/lom@xsi:schemaLocation@xmlns/general/aggregationLevel/value' => 'Description > Metadata > LOM > General > Aggregation Level > Value',
  
  // description -> LOM -> lifeCycle
  'description/metadata/lom@xsi:schemaLocation@xmlns/lifeCycle' => 'Description > Metadata > LOM > Lifecycle (Requires Template)',
  'description/metadata/lom@xsi:schemaLocation@xmlns/lifeCycle/version/string' => 'Description > Metadata > LOM > Lifecycle > Version',
  'description/metadata/lom@xsi:schemaLocation@xmlns/lifeCycle/status/source' => 'Description > Metadata > LOM > Lifecycle > Status > Source',
  'description/metadata/lom@xsi:schemaLocation@xmlns/lifeCycle/status/value' => 'Description > Metadata > LOM > Lifecycle > Status > Value',
  'description/metadata/lom@xsi:schemaLocation@xmlns/lifeCycle/contribute/role/source' => 'Description > Metadata > LOM > Lifecycle > Contribute > Role > Source',
  'description/metadata/lom@xsi:schemaLocation@xmlns/lifeCycle/contribute/role/value' => 'Description > Metadata > LOM > Lifecycle > Contribute > Role > Value',
  'description/metadata/lom@xsi:schemaLocation@xmlns/lifeCycle/contribute/entity' => 'Description > Metadata > LOM > Lifecycle > Contribute > Entity',
    
  // description -> LOM -> metaMetadata
  'description/metadata/lom@xsi:schemaLocation@xmlns/metaMetadata' => 'Description > Metadata > LOM > Meta Metadata (Requires Template)',
  'description/metadata/lom@xsi:schemaLocation@xmlns/metaMetadata/identifier/catalog' => 'Description > Metadata > LOM > Meta Metadata > Identifier > Catalog',
  'description/metadata/lom@xsi:schemaLocation@xmlns/metaMetadata/identifier/entry' => 'Description > Metadata > LOM > Meta Metadata > Identifier > Entry',
  'description/metadata/lom@xsi:schemaLocation@xmlns/metaMetadata/contribute/role/source' => 'Description > Metadata > LOM > Meta Metadata > Contribute > Role > Source',
  'description/metadata/lom@xsi:schemaLocation@xmlns/metaMetadata/contribute/role/value' => 'Description > Metadata > LOM > Meta Metadata > Contribute > Role > Value',
  'description/metadata/lom@xsi:schemaLocation@xmlns/metaMetadata/contribute/entity' => 'Description > Metadata > LOM > Meta Metadata > Contribute > Entity',
  'description/metadata/lom@xsi:schemaLocation@xmlns/metaMetadata/metadataSchema' => 'Description > Metadata > LOM > Meta Metadata > Metadata Schema',
  'description/metadata/lom@xsi:schemaLocation@xmlns/metaMetadata/language' => 'Description > Metadata > LOM > Meta Metadata > Language',
  
  // description -> LOM -> technical
  'description/metadata/lom@xsi:schemaLocation@xmlns/technical' => 'Description > Metadata > LOM > Technical (Requires Template)',
  'description/metadata/lom@xsi:schemaLocation@xmlns/technical/format' => 'Description > Metadata > LOM > Technical > Format',
  'description/metadata/lom@xsi:schemaLocation@xmlns/technical/location' => 'Description > Metadata > LOM > Technical > Location',
  
  // description -> LOM -> educational
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/interactivityType' => 'Description > Metadata > LOM > Educational > Interactivity (Requires Template)',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/interactivityType/source' => 'Description > Metadata > LOM > Educational > Interactivity > Source',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/interactivityType/value' => 'Description > Metadata > LOM > Educational > Interactivity > Value',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/learningResourceType' => 'Description > Metadata > LOM > Educational > Learning Resource Type (Requires Template)',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/learningResourceType/source' => 'Description > Metadata > LOM > Educational > Learning Resource Type > Source',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/learningResourceType/value' => 'Description > Metadata > LOM > Educational > Learning Resource Type > Value',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/interactivityLevel' => 'Description > Metadata > LOM > Educational > Interactivity Level (Requires Template)',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/interactivityLevel/source' => 'Description > Metadata > LOM > Educational > Interactivity Level > Source',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/interactivityLevel/value' => 'Description > Metadata > LOM > Educational > Interactivity Level > Value',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/intendedEndUserRole' => 'Description > Metadata > LOM > Educational > Intended End User Role (Requires Template)',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/intendedEndUserRole/source' => 'Description > Metadata > LOM > Educational > Intended End User Role > Source',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/intendedEndUserRole/value' => 'Description > Metadata > LOM > Educational > Intended End User Role > Value',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/context' => 'Description > Metadata > LOM > Educational > Context (Requires Template)',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/context/source' => 'Description > Metadata > LOM > Educational > Context > Source',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/context/value' => 'Description > Metadata > LOM > Educational > Context > Value',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/typicalAgeRange/string@language' => 'Description > Metadata > LOM > Educational > Typical Age Range',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/difficulty' => 'Description > Metadata > LOM > Educational > Difficulty (Requires Template)',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/difficulty/source' => 'Description > Metadata > LOM > Educational > Difficulty > Source',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/difficulty/value' => 'Description > Metadata > LOM > Educational > Difficulty > Value',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/typicalLearningTime/duration' => 'Description > Metadata > LOM > Educational > Typical Learning Time > Duration',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/description/string@language' => 'Description > Metadata > LOM > Educational > Description',
  'description/metadata/lom@xsi:schemaLocation@xmlns/educational/language' => 'Description > Metadata > LOM > Educational > Language',
  
  // description -> LOM -> rights
  'description/metadata/lom@xsi:schemaLocation@xmlns/rights' => 'Description > Metadata > LOM > Rights (Requires Template)',
  'description/metadata/lom@xsi:schemaLocation@xmlns/rights/cost/source' => 'Description > Metadata > LOM > Rights > Cost > Source',
  'description/metadata/lom@xsi:schemaLocation@xmlns/rights/cost/value' => 'Description > Metadata > LOM > Rights > Cost > Value',
  'description/metadata/lom@xsi:schemaLocation@xmlns/rights/copyrightAndOtherRestrictions/source' => 'Description > Metadata > LOM > Rights > Copyright (+ Other) > Source',
  'description/metadata/lom@xsi:schemaLocation@xmlns/rights/copyrightAndOtherRestrictions/value' => 'Description > Metadata > LOM > Rights > Copyright (+ Other) > Value',
  'description/metadata/lom@xsi:schemaLocation@xmlns/rights/description/string@language' => 'Description > Metadata > LOM > Rights > Description',
  
  // description -> LOM -> relation
  'description/metadata/lom@xsi:schemaLocation@xmlns/relation' => 'Description > Metadata > LOM > Relation (Requires Template)',
  'description/metadata/lom@xsi:schemaLocation@xmlns/relation/kind/source' => 'Description > Metadata > LOM > Relation > Kind > Source',
  'description/metadata/lom@xsi:schemaLocation@xmlns/relation/kind/value' => 'Description > Metadata > LOM > Relation > Kind > Value',
  'description/metadata/lom@xsi:schemaLocation@xmlns/relation/resource/description/string@language' => 'Description > Metadata > LOM > Relation > Resource > Description',
  'description/metadata/lom@xsi:schemaLocation@xmlns/relation/resource/identifier/catalog' => 'Description > Metadata > LOM > Relation > Resource > Identifier > Catalog',
  'description/metadata/lom@xsi:schemaLocation@xmlns/relation/resource/identifier/entry' => 'Description > Metadata > LOM > Relation > Resource > Identifier > Entry',
  
  // description -> LOM -> classification
  'description/metadata/lom@xsi:schemaLocation@xmlns/classification' => 'Description > Metadata > LOM > Classification (Requires Template)',
  'description/metadata/lom@xsi:schemaLocation@xmlns/classification/purpose/source' => 'Description > Metadata > LOM > Classification > Purpose > Source',
  'description/metadata/lom@xsi:schemaLocation@xmlns/classification/purpose/value' => 'Description > Metadata > LOM > Classification > Purpose > Value',
  'description/metadata/lom@xsi:schemaLocation@xmlns/classification/keyword/string@language' => 'Description > Metadata > LOM > Classification > Keyword',
  
  // manifestation
  'manifestation' => 'Manifestation (Requires Template)',
  'manifestation/identifier/catalog' => 'Manifestation > Identifier > Catalog',
  'manifestation/identifier/entry' => 'Manifestation > Identifier > Entry',
  'manifestation/name/vocabulary' => 'Manifestation > Name > Vocabulary',
  'manifestation/name/value' => 'Manifestation > Name > Value',
  'manifestation/item/identifier/catalog' => 'Manifestation > Item > Identifier > Catalog',
  'manifestation/item/identifier/entry' => 'Manifestation > Item > Identifier > Entry',
  'manifestation/item/location/uri' => 'Manifestation > Item > Location > URI',
);

// Assign the data format to the global array of OAI data formats.
$GLOBALS['views_oai_pmh']['oai_lre'] = $oai_lre;

/**
 * Definition of the OAI IMS Learning Objects Exchange (ILOX) data format. 
 */
$oai_ilox = new views_oai_pmh_metadata_type('oai_ilox');
$oai_ilox->name = t('IMS Learning Objects Exchange (ILOX)');
$oai_ilox->style_plugin = 'views_oai_pmh_ilox';
$oai_ilox->row_plugin = 'oai_ilox';
$oai_ilox->form_group_name = 'oai_ilox_fields';
$oai_ilox->help = t('Displays the fields as IMS Learning Objects Exchange elements.');
$oai_ilox->base_xml_node = new views_oai_pmh_xml_node(
  'work',
  NULL,
  array(
    'xmlns:xsi' => 'http://www.w3.org/2001/XMLSchema-instance',
    'xsi:schemaLocation' => 'http://www.imsglobal.org/xsd/lodeilox_v1p0/digital_LO       http://www.imsglobal.org/xsd/imsloilox_v1p0_ilox_work_v1p0.xsd',
    'xmlns' => 'http://www.imsglobal.org/xsd/lodeilox_v1p0/digital_LO',
  )
);
$oai_ilox->elements = array(
  'none' => t('None'),
  
  // misc elements
  '(attribute)oai_ilox@language' => t('(Attribute) Language'),
  '(attribute)oai_ilox@xsi:schemaLocation' => t('(Attribute) LOM XSI Schema Location'),
  '(attribute)oai_ilox@xmlns' => t('(Attribute) LOM XML Namespace'),
  
  // identifier
  'identifier/catalog' => 'Identifier > Catalog',
  'identifier/entry' => 'Identifier > Entry',
  
  // description
  'description*1/facet/vocabularyID' => 'Description (1) > Facet > Vocabulary ID',
  'description*1/facet/value' => 'Description (1) > Facet > Value',
  'description*1/metadata/schema' => 'Description (1) > Metadata > Schema',
  
  // description -> LOM -> general
  'description*1/metadata/schema' => 'Description (1) > Metadata > Schema',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/general' => 'Description (1) > Metadata > LOM > General (Requires Template)',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/general/identifier/catalog' => 'Description (1) > Metadata > LOM > General > Identifier > Catalog',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/general/identifier/entry' => 'Description (1) > Metadata > LOM > General > Identifier > Entry',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/general/title/string@language' => 'Description (1) > Metadata > LOM > General > Title',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/general/language' => 'Description (1) > Metadata > LOM > General > Language',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/general/description/string@language' => 'Description (1) > Metadata > LOM > General > Description',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/general/keyword' => 'Description (1) > Metadata > LOM > General > Keyword (Requires Template)',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/general/keyword/string@language' => 'Description (1) > Metadata > LOM > General > Keyword > String',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/general/structure/source' => 'Description (1) > Metadata > LOM > General > Structure > Source',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/general/structure/value' => 'Description (1) > Metadata > LOM > General > Structure > Value',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/general/aggregationLevel/source' => 'Description (1) > Metadata > LOM > General > Aggregation Level > Source',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/general/aggregationLevel/value' => 'Description (1) > Metadata > LOM > General > Aggregation Level > Value',
  
  // description -> LOM -> lifeCycle
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/lifeCycle' => 'Description (1) > Metadata > LOM > Lifecycle (Requires Template)',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/lifeCycle/version/string' => 'Description (1) > Metadata > LOM > Lifecycle > Version',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/lifeCycle/status/source' => 'Description (1) > Metadata > LOM > Lifecycle > Status > Source',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/lifeCycle/status/value' => 'Description (1) > Metadata > LOM > Lifecycle > Status > Value',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/lifeCycle/contribute/role/source' => 'Description (1) > Metadata > LOM > Lifecycle > Contribute > Role > Source',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/lifeCycle/contribute/role/value' => 'Description (1) > Metadata > LOM > Lifecycle > Contribute > Role > Value',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/lifeCycle/contribute/entity' => 'Description (1) > Metadata > LOM > Lifecycle > Contribute > Entity',
    
  // description -> LOM -> metaMetadata
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/metaMetadata' => 'Description (1) > Metadata > LOM > Meta Metadata (Requires Template)',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/metaMetadata/identifier/catalog' => 'Description (1) > Metadata > LOM > Meta Metadata > Identifier > Catalog',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/metaMetadata/identifier/entry' => 'Description (1) > Metadata > LOM > Meta Metadata > Identifier > Entry',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/metaMetadata/contribute/role/source' => 'Description (1) > Metadata > LOM > Meta Metadata > Contribute > Role > Source',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/metaMetadata/contribute/role/value' => 'Description (1) > Metadata > LOM > Meta Metadata > Contribute > Role > Value',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/metaMetadata/contribute/entity' => 'Description (1) > Metadata > LOM > Meta Metadata > Contribute > Entity',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/metaMetadata/metadataSchema' => 'Description (1) > Metadata > LOM > Meta Metadata > Metadata Schema',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/metaMetadata/language' => 'Description (1) > Metadata > LOM > Meta Metadata > Language',
  
  // description -> LOM -> technical
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/technical' => 'Description (1) > Metadata > LOM > Technical (Requires Template)',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/technical/format' => 'Description (1) > Metadata > LOM > Technical > Format',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/technical/location' => 'Description (1) > Metadata > LOM > Technical > Location',
  
  // description -> LOM -> educational
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/interactivityType' => 'Description (1) > Metadata > LOM > Educational > Interactivity (Requires Template)',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/interactivityType/source' => 'Description (1) > Metadata > LOM > Educational > Interactivity > Source',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/interactivityType/value' => 'Description (1) > Metadata > LOM > Educational > Interactivity > Value',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/learningResourceType' => 'Description (1) > Metadata > LOM > Educational > Learning Resource Type (Requires Template)',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/learningResourceType/source' => 'Description (1) > Metadata > LOM > Educational > Learning Resource Type > Source',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/learningResourceType/value' => 'Description (1) > Metadata > LOM > Educational > Learning Resource Type > Value',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/interactivityLevel' => 'Description (1) > Metadata > LOM > Educational > Interactivity Level (Requires Template)',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/interactivityLevel/source' => 'Description (1) > Metadata > LOM > Educational > Interactivity Level > Source',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/interactivityLevel/value' => 'Description (1) > Metadata > LOM > Educational > Interactivity Level > Value',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/intendedEndUserRole' => 'Description (1) > Metadata > LOM > Educational > Intended End User Role (Requires Template)',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/intendedEndUserRole/source' => 'Description (1) > Metadata > LOM > Educational > Intended End User Role > Source',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/intendedEndUserRole/value' => 'Description (1) > Metadata > LOM > Educational > Intended End User Role > Value',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/context' => 'Description (1) > Metadata > LOM > Educational > Context (Requires Template)',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/context/source' => 'Description (1) > Metadata > LOM > Educational > Context > Source',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/context/value' => 'Description (1) > Metadata > LOM > Educational > Context > Value',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/typicalAgeRange/string@language' => 'Description (1) > Metadata > LOM > Educational > Typical Age Range',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/difficulty' => 'Description (1) > Metadata > LOM > Educational > Difficulty (Requires Template)',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/difficulty/source' => 'Description (1) > Metadata > LOM > Educational > Difficulty > Source',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/difficulty/value' => 'Description (1) > Metadata > LOM > Educational > Difficulty > Value',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/typicalLearningTime/duration' => 'Description (1) > Metadata > LOM > Educational > Typical Learning Time > Duration',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/description/string@language' => 'Description (1) > Metadata > LOM > Educational > Description',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/educational/language' => 'Description (1) > Metadata > LOM > Educational > Language',
  
  // description -> LOM -> relation
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/relation' => 'Description (1) > Metadata > LOM > Relation (Requires Template)',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/relation/kind/source' => 'Description (1) > Metadata > LOM > Relation > Kind > Source',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/relation/kind/value' => 'Description (1) > Metadata > LOM > Relation > Kind > Value',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/relation/resource/description/string@language' => 'Description (1) > Metadata > LOM > Relation > Resource > Description',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/relation/resource/identifier/catalog' => 'Description (1) > Metadata > LOM > Relation > Resource > Identifier > Catalog',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/relation/resource/identifier/entry' => 'Description (1) > Metadata > LOM > Relation > Resource > Identifier > Entry',
  
  // description -> LOM -> classification
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/classification' => 'Description (1) > Metadata > LOM > Classification (Requires Template)',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/classification/purpose/source' => 'Description (1) > Metadata > LOM > Classification > Purpose > Source',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/classification/purpose/value' => 'Description (1) > Metadata > LOM > Classification > Purpose > Value',
  'description*1/metadata/lom@xsi:schemaLocation@xmlns/classification/keyword/string@language' => 'Description (1) > Metadata > LOM > Classification > Keyword',
  
  // description
  'description*2/facet/vocabularyID' => 'Description (2) > Facet > Vocabulary ID',
  'description*2/facet/value' => 'Description (2) > Facet > Value',
  'description*2/metadata/schema' => 'Description (2) > Metadata > Schema',
  
  // description -> LOM -> rights
  'description*2/metadata/lom@xsi:schemaLocation@xmlns/rights' => 'Description (2) > Metadata > LOM > Rights (Requires Template)',
  'description*2/metadata/lom@xsi:schemaLocation@xmlns/rights/cost/source' => 'Description (2) > Metadata > LOM > Rights > Cost > Source',
  'description*2/metadata/lom@xsi:schemaLocation@xmlns/rights/cost/value' => 'Description (2) > Metadata > LOM > Rights > Cost > Value',
  'description*2/metadata/lom@xsi:schemaLocation@xmlns/rights/copyrightAndOtherRestrictions/source' => 'Description (2) > Metadata > LOM > Rights > Copyright (+ Other) > Source',
  'description*2/metadata/lom@xsi:schemaLocation@xmlns/rights/copyrightAndOtherRestrictions/value' => 'Description (2) > Metadata > LOM > Rights > Copyright (+ Other) > Value',
  'description*2/metadata/lom@xsi:schemaLocation@xmlns/rights/description/string@language' => 'Description (2) > Metadata > LOM > Rights > Description',
  
  // expression
  'expression/dimension' => 'Expression > Dimension (Requires Template)',
  'expression/dimension/name/vocabularyID' => 'Expression > Dimension > Name > Vocabulary ID',
  'expression/dimension/name/value' => 'Expression > Dimension > Name > Value',
  'expression/dimension/parameter/vocabularyID' => 'Expression > Dimension > Parameter > Vocabulary ID',
  'expression/dimension/parameter/value' => 'Expression > Dimension > Parameter > Value',
  'expression/manifestation' => 'Expression > Manifestation (Requires Template)',
  'expression/manifestation/identifier/catalog' => 'Expression > Manifestation > Identifier > Catalog',
  'expression/manifestation/identifier/entry' => 'Expression > Manifestation > Identifier > Entry',
  'expression/manifestation/name/vocabulary' => 'Expression > Manifestation > Name > Vocabulary',
  'expression/manifestation/name/value' => 'Expression > Manifestation > Name > Value',
  'expression/manifestation/item/location/uri' => 'Expression > Manifestation > Item > Location > URI',
);

// Assign the data format to the global array of OAI data formats.
$GLOBALS['views_oai_pmh']['oai_ilox'] = $oai_ilox;
