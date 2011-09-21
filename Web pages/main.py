import os
import json

import web



urls = (
    '/', 'MainHandler',
    '/get_sensors.cgi', 'GetSensorsHandler',
    '/set_sensor.cgi', 'SetSensorHandler',
    '/init_sensors.cgi', 'InitSensorsHandler',
)


application = web.application(urls, globals())
working_dir = os.path.dirname(__file__)
render = web.template.render(os.path.join(working_dir, '.'))
db = web.database(dbn='sqlite', db=os.path.join(working_dir, 'testdb'))



class MainHandler(object):
    def GET(self):
        return render.index()


class GetSensorsHandler(object):
    def GET(self):
        template = 'type=%d;name=%s;enabled=%d;address=%d;threshold=%d;triggerType=%d;triggered=%d;timeTriggered=%d;value=%d'
        entries = []
        for entry in db.select('sensors'):
            entries.append(template % (
                entry.type,
                entry.name,
                entry.enabled,
                entry.address,
                entry.threshold,
                entry.triggerType,
                entry.triggered,
                entry.timeTriggered,
                entry.value
            ))
        return '<div>' + '</div><div>'.join(entries) + '</div>'


class SetSensorHandler(object):
    def GET(self):
        data = web.input(triggered=0, timeTriggered=0, value=0)
        print data
        kw = {
            'where': "address={0}".format(data.address),
            'type': int(data.type),
            'name': data.name,
            'enabled': int(data.enabled),
            'address': int(data.address),
            'threshold': int(data.threshold),
            'triggerType': int(data.triggerType),
            'triggered': int(data.triggered),
            'timeTriggered': int(data.timeTriggered),
            'value': int(data.value)
            }

        db.update('sensors', **kw)


class InitSensorsHandler(object):
    def GET(self):
        db.query("""
            CREATE TABLE sensors (
                type integer default 0,
                name text default "",
                enabled integer default 0,
                address integer primary key,
                threshold integer default 0,
                triggerType integer default 0,
                triggered integer default 0,
                timeTriggered integer default 0,
                value integer default 0
            )""")

        for i in xrange(20):
            db.insert('sensors', address=i)




if __name__ == "__main__":
    application = web.application(urls, globals())
    application.run()
