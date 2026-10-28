import csv
import datetime
import os

MQTT_USER = os.environ.get("AIRCARE_MQTT_USER")
MQTT_PASS = os.environ.get("AIRCARE_MQTT_PASS")
if not MQTT_USER or not MQTT_PASS:
    raise SystemExit(
        "Set AIRCARE_MQTT_USER and AIRCARE_MQTT_PASS environment variables."
    )

import paho.mqtt.client as mqtt

CSV_FILE = "mqtt_log.csv"
BROKER = "a8984e032441467797687728dde49d14.s1.eu.hivemq.cloud"
PORT = 8883
TOPIC = "aircare/#"

def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT Broker!")
    client.subscribe(TOPIC)

def on_message(client, userdata, msg):
    timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    topic = msg.topic
    value = msg.payload.decode()
    print(f"[{timestamp}] {topic} -> {value}")
    with open(CSV_FILE, "a", newline="") as f:
        writer = csv.writer(f)
        writer.writerow([timestamp, topic, value])

client = mqtt.Client()
client.username_pw_set(MQTT_USER, MQTT_PASS)
client.tls_set()
client.on_connect = on_connect
client.on_message = on_message
client.connect(BROKER, PORT, 60)
client.loop_forever()
