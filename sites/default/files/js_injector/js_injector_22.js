/*
jQuery(
function () {
    var t, e;
    jQuery($(function () {
        return $(".map-controller").length ? window.map = new t : void 0
    })), window.int2human = function (t) {
        switch (!1) {
        case !(t > 999 && 999999 >= t):
            return "" + Math.round(t / 1e3) + "K";
        case !(t > 999999):
            return "" + Math.round(t / 1e3 / 1e3) + "M";
        default:
            return t
        }
    }, t = L.Class.extend({
        initialize: function () {
            var t, e, i, n, s, o, r;
            return this.mapCenter = {
                lat: _.first(config.center),
                lng: _.last(config.center)
            }, this.zoom = config.zoom, this.gridSize = config.grid_size, this.stateLevel = function () {
                o = [];
                for (var t = n = _.first(config.state_level), e = _.last(config.state_level); e >= n ? e >= t : t >= e; e >= n ? t++ : t--) o.push(t);
                return o
            }.apply(this), this.gridLevel = function () {
                r = [];
                for (var t = s = _.first(config.grid_level), e = _.last(config.grid_level); e >= s ? e >= t : t >= e; e >= s ? t++ : t--) r.push(t);
                return r
            }.apply(this), this.page_size = config.page_size, this.popup_page_size = config.popup_page_size, this.markers = {
                state: [],
                grid: [],
                normal: []
            }, this.loadingStack = [], this.openPopups = [], t = this.initializeMap(), e = this.getMarkersLayer(), t.addLayer(e), i = this, this.map.on({
                dragstart: function () {
                    return i.closeAllPopups()
                },
                zoomstart: function () {
                    return i.closeAllPopups()
                },
                dragend: function () {
                    return i.updateMarkers()
                },
                zoomend: function () {
                    return i.updateMarkers()
                }
            }), this.updateMarkers()
        },
        initializeMap: function () {
            return this.map ? this.map : this.map = L.map("map", {
                center: new L.LatLng(this.mapCenter.lat, this.mapCenter.lng),
                zoom: this.zoom.start,
                layers: L.tileLayer("http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png", {
                    minZoom: this.zoom.min,
                    maxZoom: this.zoom.max,
                    attribution: 'Map data &copy; <a href="http://openstreetmap.org">OpenStreetMap</a> contributors, <a href="http://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>'
                })
            })
        },
        requestItems: function (t) {
            var e, i;
            return null == t && (t = {}), i = this, e = {
                page_size: _.isUndefined(t.page_size) ? this.page_size : t.page_size
            }, t.state && (e["sourceResource.spatial.state"] = t.state), t.bounds && (e["sourceResource.spatial.coordinates"] = ["" + t.bounds.getNorthWest().lat + "," + t.bounds.getNorthWest().lng, "" + t.bounds.getSouthEast().lat + "," + t.bounds.getSouthEast().lng].join(":")), $.ajax({
                url: window.api_search_path,
                data: e,
                dataType: "jsonp",
                cache: !0,
                beforeSend: function (e, n) {
                    return i.turnProgress(), t.beforeSend && t.beforeSend(e, n), !0
                },
                success: function (e, n, s) {
                    return e.docs = _.map(e.docs, function (t) {
                        return _.isEmpty(e.docs) ? void 0 : i.doc2point(t)
                    }), t.success && t.success(e, n, s), !0
                },
                complete: function (e, n) {
                    return i.turnProgress(!1), t.complete ? t.complete(e, n) : void 0
                }
            })
        },
        getGrid: function () {
            var t, e, i, n, s;
            return n = this, e = this.getPosition(), t = Math.abs(e.northWest.lng - e.northEast.lng) / this.gridSize.cols, i = Math.abs(e.northWest.lat - e.southWest.lat) / this.gridSize.rows, _.map(function () {
                s = [];
                for (var t = 0, e = n.gridSize.cols - 1; e >= 0 ? e >= t : t >= e; e >= 0 ? t++ : t--) s.push(t);
                return s
            }.apply(this), function (s) {
                var o;
                return _.map(function () {
                    o = [];
                    for (var t = 0, e = n.gridSize.rows - 1; e >= 0 ? e >= t : t >= e; e >= 0 ? t++ : t--) o.push(t);
                    return o
                }.apply(this), function (n) {
                    var o, r;
                    return r = new L.LatLng(e.northWest.lat - i * n, e.northWest.lng + t * s), o = new L.LatLng(r.lat - i, r.lng + t), new L.LatLngBounds(r, o)
                })
            })
        },
        getPosition: function () {
            var t;
            return t = this.map.getBounds(), {
                southWest: t.getSouthWest(),
                northWest: t.getNorthWest(),
                southEast: t.getSouthEast(),
                northEast: t.getNorthEast(),
                center: this.map.getCenter(),
                zoom: this.map.getZoom()
            }
        },
        getPositionHash: function () {
            return [this.map.getCenter().lat, this.map.getCenter().lng, this.map.getZoom()].join(":")
        },
        getBackUri: function () {
            var t, e, i;
            return e = window.location.href, window.location.href.indexOf("#") > 0 && (e = e.substr(0, window.location.href.indexOf("#"))), i = this.getPosition(), t = "" + e + "#/?lat=" + i.center.lat + "&lng=" + i.center.lng + "&zoom=" + i.zoom, encodeURIComponent(t)
        },
        getMarkersLayer: function () {
            var t;
            return t = this, this.markersLayer ? this.markersLayer : (this.markersLayer = new L.MarkerClusterGroup({
                spiderfyOnMaxZoom: !1,
                showCoverageOnHover: !1,
                zoomToBoundsOnClick: !1,
                maxClusterRadius: 45,
                iconCreateFunction: function (t) {
                    var e;
                    return e = _.reduce(t.getAllChildMarkers(), function (t, e) {
                        return t += e.count
                    }, 0), new L.DivIcon({
                        iconSize: new L.Point(20, 20),
                        className: "dot more-results",
                        html: '<div class="mapCluster"><span class="resultnumber">' + int2human(e) + "</span></div>"
                    })
                }
            }), this.markersLayer.on("clusterclick", function (e) {
                var i, n;
                switch (i = e.layer.getAllChildMarkers(), _.first(_.map(i, function (t) {
                    return t.type
                }))) {
                case "state":
                    return n = _.first(i.sort(function (t, e) {
                        return e.count - t.count
                    })), t.openStatePopup({
                        latlng: e.layer.getLatLng(),
                        state: n.data.name
                    });
                case "normal":
                    return t.openPopup({
                        latlng: e.layer.getLatLng(),
                        points: _.map(i, function (t) {
                            return t.data
                        })
                    })
                }
            }))
        },
        renderPopup: function (t) {
            var e, i;
            return i = this, e = "", _.each(t.points, function (t) {
                var n, s, o, r;
                return o = "/item/" + t.id + "?back_uri=" + i.getBackUri(), r = t.title, $.isArray(r) && (r = r[0]), n = "<h6> " + t.type + '</h6>\n<h4><a href="' + o + '" target="_blank">' + r + "</a></h4>\n<p><span> " + t.creator + '</span></p>\n<a class="ViewObject" href="' + t.url + '" target="_blank">View Object <span class="icon-view-object" aria-hidden="true"></span></a>', t.thumbnail ? (s = function () {
                    switch (!1) {
                    case t.type !== "image":
                        return window.default_images.image;
                    case t.type !== "sound":
                        return window.default_images.sound;
                    case t.type !== "video":
                        return window.default_images.video;
                    default:
                        return default_images.text
                    }
                }(), e += '<div class="box-row">\n  <div class="box-right"><img onerror="image_loading_error(this);" src="' + t.thumbnail + '" data-default-src="' + s + '" /></div>\n  <div class="box-left">' + n + "</div>\n</div>") : e += '<div class="box-row">' + n + "</div>"
            }), t.points.length > 1 && (e = '<div class="box-rows">' + e + "</div>"), t.title && (e = "<h5>" + t.title + "</h5>" + e), e
        },
        openPopup: function (t) {
            var i, n, s;
            return t.points.length > 1 && _.isEmpty(t.title) && (i = this.getClusterLocation(t.points), s = "/search?" + window.app_search_path + "&place[]=" + i, t.title = '<a href="' + s + '">' + i + "</a>"), n = new e({
                offset: new L.Point(-10, -30)
            }), n.setContent(this.renderPopup({
                title: t.title,
                points: t.points
            })), n.setLatLng(t.latlng), this.openPopups.push(n.openOn(this.map, {
                x: 20,
                y: 10
            }))
        },
        openStatePopup: function (t) {
            var e, i, n;
            return e = this, n = "/search?" + window.app_search_path + "&state[]=" + t.state, i = '<a href="' + n + '">' + t.state + "</a>", this.requestItems({
                page_size: e.popup_page_size,
                state: t.state,
                success: function (n) {
                    return e.openPopup({
                        latlng: t.latlng,
                        points: n.docs,
                        title: i
                    })
                }
            })
        },
        openGridPopup: function (t) {
            var e;
            return e = this, this.requestItems({
                page_size: e.popup_page_size,
                bounds: t.bounds,
                success: function (i) {
                    return e.openPopup({
                        latlng: t.latlng,
                        points: i.docs
                    })
                }
            })
        },
        updateMarkers: function () {
            switch (!1) {
            case !this.isStateLevel():
                return this.updateStateMarkers();
            case !this.isGridLevel():
                return this.updateGridMarkers();
            default:
                return this.updateNormalMarkers(this.map.getBounds())
            }
        },
        updateStateMarkers: function () {
            var t;
            return t = this, this.stateMarkers || (this.stateMarkers = _.map(states, function (e) {
                var i, n;
                return i = new L.DivIcon({
                    html: '<div class="mapCluster"><span class="resultnumber">' + int2human(e.count) + "</span></div>",
                    className: "dot more-results",
                    iconSize: new L.Point(20, 20)
                }), n = new L.Marker([e.lat, e.lng], {
                    icon: i
                }), n.type = "state", n.count = e.count, n.data = _.pick(e, "name"), n.on("click", function (e) {
                    return t.openStatePopup({
                        latlng: e.target.getLatLng(),
                        state: e.target.data.name
                    })
                }), n
            })), this.markers.state = this.stateMarkers, this.getMarkersLayer().addLayers(this.markers.state), this.removeGridMarkers(), this.removeNormalMarkers()
        },
        updateGridMarkers: function () {
            var t, e;
            return e = this, t = e.getPositionHash(), _.each(_.flatten(e.getGrid()), function (i) {
                return e.requestItems({
                    page_size: e.page_size,
                    bounds: i,
                    success: function (n) {
                        var s, o;
                        if (e.getPositionHash() === t) return n.count < e.page_size ? e.updateNormalMarkers(i) : (e.removeStateMarkers(i), e.removeGridMarkers(i), e.removeNormalMarkers(i), s = new L.DivIcon({
                            html: '<div class="mapCluster"><span class="resultnumber">' + int2human(n.count) + "</span></div>",
                            className: "dot more-results",
                            iconSize: new L.Point(20, 20)
                        }), o = new L.Marker(i.getCenter(), {
                            icon: s
                        }), o.count = n.count, o.type = "grid", o.on("click", function (t) {
                            return e.openGridPopup({
                                latlng: t.target.getLatLng(),
                                bounds: i
                            })
                        }), e.markers.grid.push(o), e.getMarkersLayer().addLayer(o))
                    }
                })
            })
        },
        updateNormalMarkers: function (t) {
            var e, i;
            return i = this, e = i.getPositionHash(), i.requestItems({
                bounds: t,
                success: function (n) {
                    var s;
                    if (i.getPositionHash() === e) return i.removeStateMarkers(t), i.removeGridMarkers(t), i.removeNormalMarkers(t), s = _.map(n.docs, function (t) {
                        var e, n;
                        return e = new L.DivIcon({
                            className: "dot",
                            iconSize: new L.Point(0, 0)
                        }), n = new L.Marker([t.lat, t.lng], {
                            icon: e
                        }), n.count = 1, n.type = "normal", n.data = t, n.on("click", function (t) {
                            return i.openPopup({
                                latlng: t.target.getLatLng(),
                                points: [t.target.data]
                            })
                        })
                    }), i.markers.normal = i.markers.normal.concat(s), i.getMarkersLayer().addLayers(s)
                }
            })
        },
        removeStateMarkers: function (t) {
            var e;
            return t ? (e = _.filter(this.markers.state, function (e) {
                return t.contains(e.getLatLng())
            }), this.markers.state = _.difference(this.markers.state, e)) : (e = this.markers.state, this.markers.state = []), this.getMarkersLayer().removeLayers(e)
        },
        removeGridMarkers: function (t) {
            var e;
            return t ? (e = _.filter(this.markers.grid, function (e) {
                return t.contains(e.getLatLng())
            }), this.markers.grid = _.difference(this.markers.grid, e)) : (e = this.markers.grid, this.markers.grid = []), this.getMarkersLayer().removeLayers(e)
        },
        removeNormalMarkers: function (t) {
            var e;
            return t ? (e = _.filter(this.markers.normal, function (e) {
                return t.contains(e.getLatLng())
            }), this.markers.normal = _.difference(this.markers.normal, e)) : (e = this.markers.normal, this.markers.normal = []), this.getMarkersLayer().removeLayers(e)
        },
        isStateLevel: function () {
            return _.indexOf(this.stateLevel, this.getPosition().zoom) !== -1
        },
        isGridLevel: function () {
            return _.indexOf(this.gridLevel, this.getPosition().zoom) !== -1
        },
        turnProgress: function (t) {
            return null == t && (t = !0), t ? this.loadingStack.push(!0) : this.loadingStack.pop(), _.isEmpty(this.loadingStack) ? $("#loading").hide() : $("#loading").show()
        },
        getClusterLocation: function (t) {
            var e, i, n;
            return n = {}, $.each(t, function (t, e) {
                return $.each(e.location, function (t, e) {
                    return n[e] ? n[e]++ : n[e] = 1
                })
            }), e = "", i = 0, $.each(n, function (t, n) {
                return n > i ? (e = t, i = n) : void 0
            }), e
        },
        closeAllPopups: function () {
            return _.each(this.openPopups, function (t) {
                return t._close()
            })
        },
        doc2point: function (t) {
            var e, i, n, s;
            return e = [], $.isArray(t["sourceResource.spatial.coordinates"]) ? e = function () {
                try {
                    return t["sourceResource.spatial.coordinates"][0].split(",")
                } catch (e) {
                    return i = e, []
                }
            }() : t["sourceResource.spatial.coordinates"] && (e = t["sourceResource.spatial.coordinates"].split(",")), n = t["sourceResource.spatial.name"], n instanceof Array || (n = [n]), s = {
                id: t.id,
                title: t["sourceResource.title"] || t.id,
                thumbnail: t.object,
                type: t["sourceResource.type"] || "",
                creator: t["sourceResource.creator"] || "",
                location: n,
                url: t.isShownAt,
                lat: e.shift(),
                lng: e.shift()
            }
        }
    }), e = L.Popup.extend({
        _initLayout: function () {
            var t, e;
            return this._container = e = L.DomUtil.create("div", "mapBox"), L.DomEvent.disableClickPropagation(e), t = L.DomUtil.create("a", "closePopUp", e), t.href = "#close", t.innerHTML = "&#215;", L.DomEvent.on(t, "click", this._onCloseButtonClick, this), this._contentNode = L.DomUtil.create("div", "boxInner", e), L.DomEvent.on(this._contentNode, "mousewheel", L.DomEvent.stopPropagation), this._tip = L.DomUtil.create("div", "mapArrow", e)
        },
        _updateLayout: function () {
            var t;
            return t = this._contentNode, this._containerWidth = t.offsetWidth, this._container.style.bottom = "50px", this._container.style.left = "50px"
        },
        openOn: function (t, e) {
            return null == e && (e = {
                x: 10,
                y: 20
            }), L.Popup.prototype.openOn.call(this, t), this._container.style.left = this._containerLeft + e.x + "px", this._container.style.bottom = this._containerBottom + e.y + "px", this
        }
    })
}.call(this));
//);
*/