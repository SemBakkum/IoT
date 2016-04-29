var data = {
    labels: ["Maandag 25 april", "Dinsdag 26 april", "Woensdag 27 april", "Donderdag 28 april", "Vrijdag 29 april", "Zaterdag 30 april", "Zondag 1 mei"],
    datasets: [
        {
            label: "Voltaplein 55",

            // The properties below allow an array to be specified to change the value of the item at the given index
            // String  or array - the bar color
            backgroundColor: "rgba(231,35,74, 0.5)",

            // String or array - bar stroke color
            borderColor: "rgba(231,35,74, 1)",

            // Number or array - bar border width
            borderWidth: 3,
            hoverBackgroundColor: "rgba(53,64,82,0.5)",
            hoverBorderColor: "rgba(53,64,82,1)",

            // The actual data
            data: [4, 8, 2, 0, 6, 4, 7],

            // String - If specified, binds the dataset to a certain y-axis. If not specified, the first y-axis is used.
            yAxisID: "y-axis-0",
        },
        {
            label: "Voltastraat 1",
            backgroundColor: "rgba(0,172,80,0.5)",
            borderColor: "rgba(0,172,80,1)",
            borderWidth: 3,
            hoverBackgroundColor: "rgba(53,64,82,0.5)",
            hoverBorderColor: "rgba(53,64,82,1)",
            data: [2, 4, 4, 1, 8, 7, 9]
        }
    ]
};



var ctx = document.getElementById("myChart");

var myBarChart = new Chart(ctx, {
    type: 'bar',
    data: data,
    responsive: true,
    position: 'bottom',
    defaultFontColor: "#FFFFFF"
});

