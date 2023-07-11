const http = require('http');
const express = require('express');
const app = express();
const mongoose = require('mongoose');
const server = http.createServer(app);
require('dotenv').config();
app.use(express.urlencoded({ extended: true }));

mongoose.connect(process.env.mongoString);
const database = mongoose.connection

database.on('error', (error) => {
    console.log(error)
})

database.once('connected', () => {
    console.log('Database Connected');
})

const production_line_model=require("./models/production_line_M");

app.get('/setting_phase_times', (req, res) => {
  res.sendFile(__dirname + '/determining_phases_times.html');
});


app.post('/saving_phase_times', async (req, res) => {
  try {
    const heating_time = req.body.heating_time;
    const light_test_time = req.body.light_test_time;
    const activation_component_time = req.body.activation_component_time;
    const delay_time = req.body.delay_time;


    const existingData = await production_line_model.find().limit(1);
    if (existingData.length > 0) {
      const firstRecord = existingData[0];
      firstRecord.heating_time = heating_time;
      firstRecord.light_test_time = light_test_time;
      firstRecord.activation_component_time = activation_component_time;
      firstRecord.delay_time = delay_time;
      await firstRecord.save();
    } else {
      const modelData = new production_line_model({
        heating_time: heating_time,
        light_test_time: light_test_time,
        activation_component_time: activation_component_time,
        delay_time: delay_time
      });
      await modelData.save();
    }

    res.status(200).json({ message: 'זמני השלבים נשמרו בהצלחה!' });
  } catch (error) {
    console.error('שגיאה בשמירת זמני השלבים:', error);
    res.status(500).json({ message: 'שגיאה בשמירת זמני השלבים' });
  }
});

app.get('/get_phase_time', async (req, res) => {
  try {
    const row = await production_line_model.findOne().sort({ createdAt: 1 }).exec();
    if (row) {
      const heating_time = row.heating_time;
      const light_test_time = row.light_test_time;
      const activation_component_time = row.activation_component_time;
      const delay_time = row.delay_time;
      console.log(row);
      
      const jsonData = {
        heating_time: heating_time,
        light_test_time: light_test_time,
        activation_component_time: activation_component_time,
        delay_time: delay_time
      };
      
      res.json(jsonData);
    } else {
      console.log('לא נמצאה שורה במסד הנתונים');
      res.status(404).json({ message: 'לא נמצאה שורה במסד הנתונים' });
    }
  } catch (err) {
    console.error('שגיאה בלקיחת השורה:', err);
    res.status(500).json({ message: 'שגיאה בלקיחת השורה' });
  }
});

server.listen(3000, '1.1.1.1', () => {
  console.log('Server is running on http://1.1.1.1:3000/');
});


