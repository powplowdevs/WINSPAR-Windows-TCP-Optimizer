const express = require('express');
const app = express();
const port = 3000;

app.use(express.static('public'));

app.get('/speed-test', (req, res) => {
    const imageAddr = "ADD LINK HERE";
    const downloadSize = 7300000;

    function MeasureConnectionSpeed() {
        return new Promise((resolve, reject) => {
            let startTime, endTime;
            let download = new Image();
            download.onload = function () {
                endTime = (new Date()).getTime();
                resolve(showResults());
            }

            startTime = (new Date()).getTime();
            let cacheBuster = "?nnn=" + startTime;
            download.src = imageAddr + cacheBuster;

            function showResults() {
                let duration = (endTime - startTime) / 1000;
                let bitsLoaded = downloadSize * 8;
                let speedBps = (bitsLoaded / duration).toFixed(2);
                let speedKbps = (speedBps / 1024).toFixed(2);
                let speedMbps = (speedKbps / 1024).toFixed(2);
                let result = {
                    speedBps: parseFloat(speedBps),
                    speedKbps: parseFloat(speedKbps),
                    speedMbps: parseFloat(speedMbps)
                };

                return result;
            }
        });
    }

    MeasureConnectionSpeed().then(speedData => {
        res.json(speedData);
    });
});

app.listen(port, () => {
    console.log(`Server listening at http://localhost:${port}`);
});
