
void handleRoot() {
  const char htmlTemplate[] PROGMEM = R"html(
    <!DOCTYPE html>
    <html>

    <head>
      <meta charset='utf-8' />
      <title>2023 Solar Boat Race</title>
      <meta name='viewport' content='initial-scale=1,maximum-scale=1,user-scalable=no' />
      <script src='https://api.mapbox.com/mapbox-gl-js/v2.5.0/mapbox-gl.js'></script>
      <link href='https://api.mapbox.com/mapbox-gl-js/v2.5.0/mapbox-gl.css' rel='stylesheet' />

    <style>
      body { margin: 0; padding: 0; }
      #map-container { position: relative; width: 100%; height: 100vh; }
      #map { position: absolute; top: 0; bottom: 0; width: 100%; }
#map - heading {
        position: absolute;
        top: 10px; /* Adjust this value to position the title as needed */
        left: 50%;
        transform: translateX(-50%);
        background-color: rgba(255, 255, 255, 0.7);
        padding: 5px;
        font-size: 14px;
        font-weight: bold;
        margin: 0;
        z-index: 1000; /* Adjust this value if necessary */
      }
    </style>




      </head>

      <body>
        <div id="map-container">
          <div id="map-heading">2023 Solar Boat Race</div>
          <div id="map"></div>
        </div>
      
        <script>
          mapboxgl.accessToken = 'pk.eyJ1IjoibmVteXN3aWwiLCJhIjoiY2xnNDB4M2JwMGFobzNwbTJvMXhlYzZuZyJ9._ADtgy5IL1Ux9XGht7lR_A';
          var map;
          var centerSet = false;  // To track if the initial center is set

            map = new mapboxgl.Map({
              container: 'map',
              style: 'mapbox://styles/mapbox/streets-v11',
              center: [25.671499, -34.003654],
              zoom: 17
            });
          

          // Create an array of GPS data points, each with a unique color and shape
          var gpsData = [
            { coords: [25.67007, -33.091799], color: '#ff0000', shape: 'circle' },
            { coords: [25.67005, -33.091799], color: '#00ff00', shape: 'triangle' },
            { coords: [25.67003, -33.091799], color: '#0000ff', shape: 'square' },
            { coords: [25.67001, -33.091799], color: '#ffff00', shape: 'circle' },
            { coords: [25.67000, -33.091799], color: '#ff00ff', shape: 'triangle' },
            { coords: [25.66999, -33.091799], color: '#00ffff', shape: 'square' }
          ];
          // Create an array of markers for the GPS data points
          var markers = gpsData.map(function (point) {
            var el = document.createElement('div');
            el.style.width = '20px';
            el.style.height = '20px';
            el.style.borderRadius = point.shape === 'circle' ? '50%' : '0%';
            el.style.backgroundColor = point.color;
            return new mapboxgl.Marker({ element: el })
              .setLngLat(point.coords)
              .addTo(map);
          });


          function updateMap() {
            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function () {
              if (xhr.readyState === 4 && xhr.status === 200) {

                var xmlDoc = xhr.responseXML;
                var rows = xmlDoc.getElementsByTagName('row');
                for (var i = 0; i < rows.length; i++) {
                  var deviceId = rows[i].getAttribute('deviceid');
                  var latitude = parseFloat(rows[i].getAttribute('latitude'));
                  var longitude = parseFloat(rows[i].getAttribute('longitude'));

                  gpsData[i].coords[0] = parseFloat(rows[i].getAttribute('longitude'));// update longitude 
                  gpsData[i].coords[1] = parseFloat(rows[i].getAttribute('latitude'));  // update  latitude 

                  console.log('Device info : ', deviceId, latitude, longitude);

                }

                // Update the marker positions
                for (var i = 0; i < markers.length; i++) {
                  markers[i].setLngLat(gpsData[i].coords);
                }
                // Recenter the map to show the updated coordinates
                map.flyTo({
                  center: gpsData[0].coords, // Set the center to the first GPS data point
                  zoom: 20, // Optional: You can adjust the zoom level as needed
                  speed: 1.5, // Optional: Controls the flyTo animation speed (default is 1.2)
                  curve: 1, // Optional: Controls the flyTo animation curve (default is 1)
                  easing: function (t) {
                    return t;
                  } // Optional: Customize the easing function for the flyTo animation (default is a cubic bezier curve)
                });
                
              }
            };
            xhr.open('GET', '/getGPSData', true);
            xhr.send();
          }

          window.onload = function () {
          
            setInterval(updateMap, 1000);
          };
        </script>

      </body>

      </html>




  )html";

  server.send(200, "text/html", String(F(htmlTemplate)));
}

void handleGPSData() {
  String xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
  xml += "<gps-data>";
  for (int i = 0; i < numDevices; i++) {
    xml += "<row deviceid=\"" + String(i) + "\" latitude=\"" + String(devices[i].latitude, 6) + "\" longitude=\"" + String(devices[i].longitude, 6) + "\" />";
  }
  xml += "</gps-data>";

  server.send(200, "text/xml", xml);
}


void setupWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

    // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("Open http://");
  Serial.println(ip);
}
