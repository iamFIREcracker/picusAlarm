var SensorManager = (function() {
  return {
    onReady: function() {
      this.addEmptySensor();
      $('#sensors_save').click(this.saveSensors);
    },

    addEmptySensor: function() {
      $('#empty_sensor').append(Sensor());
    },

    addSensor: function($sensor) {
      $('#configured_sensors').append($sensor);
    },

    delSensor: function($sensor) {
      $sensor.find('input[name=sensor_enabled]').val(0);
      $sensor.find('input[type=button]').remove();
      $sensor.addClass('deleted');
    },

    loadSensors: function() {
      $.ajax({
        'type': 'GET',
        'url': '/get_sensors.cgi',
        'dataType': 'text',
        'cache': false,
        'success': function(data, success) {
          $(data).each(function(index) {
            var q = $(this).text();
            var splitted = q.split(';');
            var i;
            var data = {};

            for (i in splitted) {
              kv = splitted[i].split('=');
              data[kv[0]] = kv[1];
            }

            var found = 0;
            $('#configured_sensors input[name=sensor_port]').each(function(index) {
              if ($(this).val() == data.address) {
                $(this).parent().find('span.sensor_value').text(data.value);
                if (data.triggered == '1') {
                  $(this).parent().addClass('triggered');
                } else {
                  $(this).parent().removeClass('triggered');
                }
                found = 1;
              }
            });

            if (!found && data.enabled != 0) {
              SensorManager.addSensor(Sensor(data));
            }

          });
        },
        'error': function(data, failure) {
          Logger.warning('Something went wrong: ops ;-)');
        }
      });

      setTimeout(SensorManager.loadSensors, 500);
    },

    saveSensors: function() {
      $('#configured_sensors .sensor').each(function(elem) {
        var $sensor = $(this);
        var q = 'name=' + $sensor.find('input[name=sensor_name]').val() + '&'
              + 'type=' + $sensor.find('input[name=sensor_type]:checked').val() + '&'
              + 'address=' + $sensor.find('input[name=sensor_port]').val() + '&'
              + 'threshold=' + $sensor.find('input[name=sensor_threshold]').val() + '&'
              + 'triggerType=' + $sensor.find('input[name=switching_activity]:checked').val() + '&'
              + 'enabled=' + $sensor.find('input[name=sensor_enabled]').val() + '&';

        $.ajax({
          'type': 'GET',
          'url': '/set_sensor.cgi?' + q,
          'dataType': 'text',
          'cache': false,
          'success': function(data, success) {
            if ($sensor.hasClass('deleted'))
              $sensor.remove();
          },
          'error': function(data, failure) {
            Logger.error('Opps, something went wrong!!!');
          }
        });
      });

      return false;
    },
  }
})();


var Sensor = (function(data) {
  var name_hint = 'Sensor name';
  var port_hint = 'Sensor port';
  var name = name_hint;
  var address = port_hint;
  var type = '0';
  var threshold = 511;
  var switching = '0';
  var value = 'N/A';
  var submit = 'Add';
  var enabled = '1';
  var triggered = '0';
  if (data) {
    name = data.name;
    address = data.address;
    type = data.type;
    threshold = data.threshold;
    switching = data.triggerType;
    value = data.value;
    enabled = data.enabled;
    triggered = data.triggered;
    submit = 'Delete';
  }

  var $sensor = $('<form class="sensor ' + ((triggered == '1') ? 'triggered' : '') + '">'
                + '<input type="text" name="sensor_name" value="' + name + '" ' + (data ? 'readonly="readonly"' : '') + ' class="' + (data ? '' : 'hint') + '"/>'
                + '<input type="text" name="sensor_port" value="' + address + '" ' + (data ? 'readonly="readonly"' : '') + ' class="' + (data ? '' : 'hint') + '"/>'
                + '<br />'
                + 'Sensor type: <label><input type="radio" name="sensor_type" value="0" ' + ((type == "0") ? 'checked="checked"' : '') + ' />Digital</label>'
                + '<label><input type="radio" name="sensor_type" value="1" ' + ((type == "1") ? 'checked="checked"' : '') + ' /> Analog</label>'
                + '<span class="range" style="display:' + ((type == "0") ? 'none' : 'inline') + '">(Threshold: <input type="range" name="sensor_threshold" min="0" max="1023" value=' + threshold + ' /><span class="range_value">' + threshold + '</span>)</span>'
                + '<br />'
                + 'Triggered: <label><input type="radio" name="switching_activity" value="0" ' + ((switching == "0") ? 'checked="checked"' : '') + ' />Rising edge</label>'
                + '<label><input type="radio" name="switching_activity" value="1" ' + ((switching == "1") ? 'checked="checked"' : '') + ' />Falling edge</label>'
                + '<br />'
                + '<span class="value" style="display: ' + ((value == 'N/A') ? 'none' : 'inline') + '">Value: <span class="sensor_value">' + value + '</span></span>'
                + '<input type="hidden" name="sensor_enabled" value="' + enabled + '" />'
                + '<br />'
                + '<input type="button" value="' + submit + '" class="submit" />'
                + '</form>');

  $sensor.find('input[type=text]').bind({
    focus: function() {
      var $this = $(this);
      var hint = (($this.attr('name') == 'sensor_name') ? name_hint : port_hint);

      if ($this.val() == hint) {
          $this.removeClass('hint');
          $this.val('');
      }
    },
    blur: function() {
      var $this = $(this);
      var hint = (($this.attr('name') == 'sensor_name') ? name_hint : port_hint);

      if ($this.val().length == 0) {
          $this.addClass('hint');
          $this.val(hint);
      }
    }
  })

  $sensor.find('input[name=sensor_type]').click(function() {
    var $threshold = $sensor.find('span.range');

    if ($(this).val() == '0') {
      $threshold.css('display', 'none');
    } else {
      $threshold.css('display', 'inline');
    }
  });

  $sensor.find('input[name=sensor_threshold]').change(function() {
    $sensor.find('.range_value').text(this.value);
  });

  $sensor.find('input[type=button]').click(function() {
    var $this = $(this);

    if ($this.val() == 'Add') {
      var $parent = $this.parent();
      var name = $parent.find('input[name=sensor_name]').val();
      var address = parseInt($parent.find('input[name=sensor_port]').val());

      if (name.length == 0 || name === name_hint) {
        Logger.warning('Invalid name: ' + name);
        return;
      }

      if (typeof address !== 'number' || !isFinite(address)) {
        Logger.warning('Invalid port: ' + address);
        return;
      }

      $parent.find('input[type=text]').removeClass('hint').unbind({
        'focus': null,
        'blur': null
      });

      $parent.find('.value').css('display', 'inline');
      $parent.find('input[type=text]').attr('readonly', 'readonly');

      $this.val('Delete');
      SensorManager.addSensor($sensor);
      SensorManager.addEmptySensor();

    } else {
      SensorManager.delSensor($sensor);
    }
    Logger.info('Save configuration to commit changes...');
  });

  return $sensor;
});
