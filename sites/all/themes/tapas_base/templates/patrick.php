
<?php

// Fake content

$str = <<<'EOD'
sea_creatures2.xml Title (DC Title):  Epithalamion Display Title:  Epithalamion Alternate Titles (DC Alternative):  Julia&#039;s monsters Author (TAPAS):  Patrick Rashleigh Collection:  pr Project:  Patrick&#039;s <strong>test</strong> project DC Coverage-Spatial:  Asia <strong>Bhutan</strong> Publication Date of Source (DC Date):  2012-01-01 00:00:00 DC Rights:  CC-BY DC Identifier:  https://www.ptapascit.services.brown.edu/node/1129 Short Description (DC Description):  Julia&#039;s fantastical collection of sea creatures TEI Full Text:<strong>Epithalamion Freely</strong> available Julia Flanders Sea Creatures Ridgeback Press 2012 added markup When the tide goes out and the <strong>glass creatures</strong> are left behind on the beach sand, their surfaces dessicate and rupture. They absorb light into the pores and hollows of their skin, where it concentrates and granulates as their colors darken. When the sun is low and its light is thick and soft and indirect, the glass creatures float above the surface of the water, lapping its ripples with their own, lit from above and below by the sun and its reflected light, so that they are infused with… At dusk, the sun shows as an afterthought and the glass creatures are free to move between air and water. They rise, dripping <strong>light</strong>, and skim across the boundary layer; when they dive, they carry layers of <strong>light</strong> with them that disperse from <strong>their</strong>
EOD;

// User options

$MAX_SNIPPET_LENGTH = 30; // Number of characters

// Map of field labels with display labels

$FIELD_NAMES = array(
	'Author \(TAPAS\):' => 'Author',
	'Project Summary:' => 'Project Summary',
	'Institution:' => 'Institution',
	'Collection:' => 'Collection',
	'Collection Description:' => 'Collection Description',
	'Project:' => 'Project',
	'Title \(DC Title\):' => 'Title',
	'Display Title:' => 'Title for display',
	'Alternate Titles \(DC Alternative\):' => 'Alternate title',
	'Author \(DC Creator\):' => 'Author',
	'DC Contributor:' => 'Contributor',
	'Location:' => 'Located at',
	'DC Coverage-Spatial:' => 'About geographical region',
	'Publication Date of Source \(DC Date\):' => 'Date of publication',
	'DC Publisher:' => 'Published by',
	'Language \(DC Language\):' => 'Language',
	'DC Rights:' => 'Rights',
	'Rights Granted Additional Info:' => 'Rights Granted',
	'DC Identifier:' => 'Identifier',
	'DD Classification \(DC Subject\):' => 'Classified as',
	'Getty AAT Genre \(DC Subject\):' => 'Subject (Getty)',
	'LOC Classification \(DC Subject\):' => 'Subject (Library of Congress)',
	'User Subject Tags \(DC Subject\):' => 'Subject',
	'Source \(DC Source\)' => 'Source',
	'DC Source \(Full Text\)' => 'Source',
	'Short Description \(DC Description\):' => 'Description',
	'TEI Full Text:' => 'Document text'
);

// Parse out fields 

$labelRegEx = join('|', array_keys ( $FIELD_NAMES ));
preg_match_all("/(?P<label>$labelRegEx)(?P<value>.*?)(?=$|$labelRegEx)/smu", $str, $matches);

// Iterate through matched fields

$table =''; // Collect listing of matching fields here (an array would be cleaner)

$header = '';
$project = '';
$collection = '';

for ($i = 0; $i < count($matches[label]); ++$i) {

	// Current field label & value

	$label = $matches[label][$i];
	$value = $matches[value][$i];

	// Grab fields that will be displayed & look for <strong> tag in all other fields

	if ($label == "Display Title:") {
		$header = $value;
	} elseif ($label == "Project:") {
		$project = $value;
	} elseif ($label == "Collection:") {
		$collection = $value;
	} elseif (preg_match ( '/<strong>.*?<\/strong>/' , $value )) {

		// If <strong> tag is in TEI full text, reduce to snippets separated by '...'

		if ($label == "TEI Full Text:") {
			preg_match_all ( "/(?:\b|^).{0,$MAX_SNIPPET_LENGTH}<strong>.*?<\/strong>.{0,$MAX_SNIPPET_LENGTH}(?:\b|$)/" , $value, $snippets );
			$value = '...' . join("...<br />... ", $snippets[0]) . '...';
		}

		// Add matching fields to table

		$table .= "<tr><th valign='top'>" . $FIELD_NAMES[$label] . "</th><td>" . $value . "</td></tr>\n";
	}
}

// Print output

print "<html>\n";
print "<head><style> th { vertical-align: top; text-align: left; } th, td {  padding: 0.6em; } body { padding: 6% 10%; font-family: sans-serif }</style></head>\n";
print "<h1>" . $header . "</h1>";
print "<p>Part of collection <em>" . $collection . "</em> of project <em>" . $project . "</em></p>";
print "<table border='0'>" . $table . "</table>";
print "</html>\n";
