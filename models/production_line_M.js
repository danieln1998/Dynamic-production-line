const mongoose = require('mongoose');


const production_line = new mongoose.Schema({
  heating_time:{
        required: true,
        type: Number
    },
    light_test_time:{
      required: true,
        type: Number
    },
    activation_component_time:{
      required: true,
      type: Number
    },
    delay_time:{
      required: true,
      type: Number
    }
  });

module.exports =mongoose.model('production_lines', production_line);