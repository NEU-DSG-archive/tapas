/*
var markers = MarkerClusterGroup({
    iconCreateFunction: function(cluster) {
        return DivIcon({ html: '<b>' + cluster.getChildCount() + '</b>' });
    }
});
/*
a.layer.IconCreateFunction: function (cluster) {
		var childCount = cluster.getChildCount();
//                var countmarkers = cluster.getChildCount();
				var childCount = 0;
alert(document.getElementById('term-1').innerHTML);
for (var i=0,len=countmarkers; i<len; i++)
{ 
var divID = "term-" + i;
alert(divID);
alert(document.getElementById(divID).innerHTML);
childCount = childCount + document.getElementById(divID).children[0][1];
}
		var c = ' marker-cluster-';
		if (childCount < 10) {
			c += 'small';
		} else if (childCount < 100) {
			c += 'medium';
		} else {
			c += 'large';
		}

		return new L.DivIcon({ html: '<div><span>' + childCount + '</span></div>', className: 'marker-cluster' + c, iconSize: new L.Point(40, 40) });
	},
*/