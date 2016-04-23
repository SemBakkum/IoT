<?php

// Check if this is a post request
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
  $value = $_POST['value'];
  $status = ($value === '1') ? 'open' : 'closed';

  // Status
  $file = fopen("door.json", "w+") or die("can't open file");
  fwrite($file, '{"door": "' . $status . '"}');
  fclose($file);

  // History
  $content = json_decode(file_get_contents('history.json'));
  $content[] = (['status' => $status, 'created_at' => time()]);
  $history = fopen("history.json", "w+") or die("can't open file");
  fwrite($history, json_encode($content));

} else {

  $door = $_GET['door'];
  if($door == "open") {  
    $file = fopen("door.json", "w+") or die("can't open file");
    fwrite($file, '{"door": "open"}');
    fclose($file);
  } 
  else if ($door == "closed") {  
    $file = fopen("door.json", "w") or die("can't open file");

    $new = '{"door": "closed"}';
    fwrite($file, $new);
    fclose($file);
  }


  $content = json_decode(file_get_contents('door.json'));
  $doorStatus = $content->door;
}
  ?>

  <html>  
    <head>      
      <meta charset="utf-8">
      <meta http-equiv="X-UA-Compatible" content="IE=edge">
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <link rel="stylesheet" type="text/css" href="style.css">
      <script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/1.0.2/Chart.js"></script>
      <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.0.0-beta1/jquery.min.js"></script>
      <script src="https://cdnjs.cloudflare.com/ajax/libs/moment.js/2.13.0/moment.min.js"></script>
      <title>Close the door!</title>

    </head>
    <body>
      <div class="content-wrapper">

        <div class="melding">
          <h1>
            <?php
              if($door=="open") {
                echo("Door is open too long!");
              }
              else if ($door=="closed") {
                echo("The alarm is off.");
              }
              else{
                echo ("Good day to you.");
              }
            ?>
          </h1>
        </div>
        <div>
        </div>
        <div class="btns">
          <!-- <div class="btnOn">
            <a href="?door=open">Turn on alarm</a>
          </div> -->
          <div class="btnOff">
            <a href="?door=closed">Turn off alarm</a>
          </div>
        </div>
        <h1 id="chart">Times the door opened or closed</h1>
        <canvas id="myChart"></canvas>
    </div>
    </body>

    <script>

$.ajax({
 url:'history.json',
 success: function (data) {
   var values = data.map(function(d) { return (d.status === 'open') ? 1 : 0 });
   var timestamps = data.map(function(d) { return moment.unix(d.created_at).format('MMMM Do YYYY, H:mm:ss ') });

   var data = {
       labels: timestamps,
       datasets: [
       {
         fillColor: "rgba(102,102,102,0.2)",
         strokeColor: "rgba(25,197,137,1)",
         pointColor: "rgba(102,102,102,1)",
         pointStrokeColor: "#19c589",
         pointHighlightFill: "#666",
         pointHighlightStroke: "rgba(102,102,102,1)",
         data: values
       }
       ]
     };

     var ctx = document.getElementById("myChart").getContext("2d");
     var myLineChart = new Chart(ctx).Bar(data);

 }
});

    </script>

  </html>

<?php
}
?>