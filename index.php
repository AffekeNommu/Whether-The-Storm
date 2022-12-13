<html lang="en">
   <head>
      <title>Whether</title>
      <!--Javascript libraries and corresponding styles-->
      <!-- jQuery -->
      <script src="https://code.jquery.com/jquery-3.4.1.min.js" crossorigin="anonymous"></script>
      <!-- chart.js -->
      <script src="https://cdn.jsdelivr.net/npm/chart.js@2.9.3/dist/Chart.min.js" integrity="sha256-R4pqcOYV8lt7snxMQO/HSbVCFRPMdrhAFMH+vr9giYI=" crossorigin="anonymous"></script>
      <!-- Moment.js -->
      <script src="https://cdnjs.cloudflare.com/ajax/libs/moment.js/2.18.1/moment.min.js"></script>
      <!-- Make it display big on a phone -->
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <style type="text/css"></style>
    </head>
    <body>
      <h1>Whether the storm?</h1>
      <canvas id="myChart" width="400" height="400"></canvas>
      </body>
</html>
<script>
//vars
var times=[];
var temps=[];
var hums=[];
var days=1;

//functions
function loadData(){
  readings=$.ajax({
    url: './function.php',
    type: 'get',
    contentType: "application/json",
          dataType: 'json',
    data: {Action:'get', Days:days},
    success: function(data) {
        //called when successful
    //console.log(temps.responseJSON); //good we have the data in JSON
    //make a nice set of 3 arrays for the chart to use - labels, temp and hum
    $.each(readings.responseJSON, function (idx, val) {
      temps.push(val['Temperature'])
      timetemp=moment(val['TimeStamp']+'Z')
      times.push(timetemp.format('DD/MM HH:mm'))
      hums.push(val['Humidity'])
    });
    chartload();
    }
  })
}
function chartload(){
  var ctx = document.getElementById('myChart').getContext('2d');
  var myChart = new Chart(ctx, {
    type: 'line',
    data: {
      labels: times,
      datasets: [{
        label: 'Temperature',
        type: 'line',
        data: temps,
        backgroundColor: "rgba(255, 71, 26,0.4)",
        yAxisID: 'degrees'
      }, {
        label: 'Humidity',
        data: hums,
        backgroundColor: "rgba(0, 153, 255,0.4)",
        yAxisID: 'percent'
      }]
    },
            options:{
      scales:{
        yAxes:[
          {
              id: "degrees",
              ticks: {
                beginAtZero: false,
              },
              scaleLabel: {
                display: true,
                labelString: '℃'
              }
          },
          {
              id: "percent",
              position: 'right',
              ticks: {
                beginAtZero: true,
              },
              scaleLabel: {
                display: true,
                labelString: '%'
              }
          },
        ]
      }
    }
  });
}

$( document ).ready(function() {
loadData();
$('#days').change(function() {
  days=document.getElementById('days').value
  myChart.removeData();
  loadData();
});
});
//end of the page js
</script>
