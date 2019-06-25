
function addForecast(){
    var lat = document.getElementById("lat").value;
    var lon = document.getElementById("lon").value;
    $.ajax({
            method: "GET",
            url: "https://forecast.weather.gov/MapClick.php",
            data: {   
                "lat": lat,
                "lon": lon,
                "FcstType": 'json'
            },
            success: function(weather){    
                var div = document.createElement("div");             
                var button = document.createElement("button");
                button.className = "deleter";
                div.className = "forecast";
                button.addEventListener("click",function(){ cntnr.removeChild(div); });
                var h1 = document.createElement("h1");
                h1.textContent = weather.location.areaDescription;
                h1.className = "head1";
                var div2 = document.createElement("div");
                div2.className = "whitepart";
                var h2 = document.createElement("h2");
                h2.textContent = weather.currentobservation.name;
                h2.className = "head2";
                var info = document.createElement("p");
                var br = document.createElement("br");
                info.className = "info";
                var time = document.createElement("span");
                time.textContent = weather.creationDateLocal;
                var temp = document.createElement("span");
                temp.textContent = weather.data.temperature[0] + " degrees";
                var desc = document.createElement("span");
                desc.textContent = weather.data.weather[0];
                info.appendChild(time);
                info.appendChild(br);
                info.appendChild(temp);
                info.appendChild(br);
                info.appendChild(desc);
                info.appendChild(br);
                var img = document.createElement("img");
                img.className = "wimg";
                img.setAttribute("src", weather.data.iconLink[0]);
                div.appendChild(button);
                div.appendChild(h1);
                div2.appendChild(h2);
                div2.appendChild(info);
                div2.appendChild(img);
                div.appendChild(div2);
                var cntnr = document.getElementById("container")
                cntnr.appendChild(div);
            },
            error: function(error){
                console.log(error);
            }
    });
}

