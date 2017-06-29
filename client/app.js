var ctx = document.getElementById("myChart").getContext('2d');
var socket = io('http://localhost:3000');
var myChart = null;
var myValues = [];
var myRange = 130;

// crea grafico con i valori di data
myChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: ["Prova"],
        datasets: [{
            label: 'Distanza',
            data: [0],
            backgroundColor: [
                'rgba(255, 99, 132, 0.0)'
            ],
            borderColor: [
                'rgba(0,0,255,1)'
            ],
            borderWidth: 2
        }]
    },
    options: {
        scales: {
            yAxes: [{
                ticks: {
                    beginAtZero:true
                }
            }]
        }
    }
});

function addData(chart, label, data) {
    chart.data.labels.push(label);
    chart.data.datasets.forEach((dataset) => {
        dataset.data.push(data);
    });
    chart.update();
}

function checkValue(value) {
    var sum = 0;
    var med = 0;
    for(var v in myValues) {
        if(parseInt(myValues[v]) > 1) {
            sum = sum + parseInt(myValues[v]);
        }
    }
    med = Math.floor(sum / myValues.length);
    if(value < med + myRange && value > med - myRange) {
        return true;
    } else {
        return false;
    }
}

socket.on('connected', function(){
    console.log("Socket connected");
});
socket.on('disconnect', function(){
    console.log("Socket disconnected");
});
socket.on('data', function (data) {
    console.log("Received data from server (Arduino)");
    console.log(data);
    myValues.push(data.trim());

    if(checkValue(parseInt(data))) {
        addData(myChart, data.toString(), data);
    }
    
});

