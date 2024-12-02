import paho.mqtt.client as mqtt
import time

# Connection details (replace with your Render values)
broker_address = "broker render"  # aca hubo un error al conectar el render 
port = 1883
user = ""
password = ""


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to broker with result code " + str(rc))
        client.subscribe(topic)
    else:
        print("Connection failed: " + str(rc))

def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.payload))

client = mqtt.Client()

client.on_connect = on_connect
client.on_message = on_message

client.username_pw_set(user, password)

# Connect with error handling
try:
    client.connect(broker_address, port)
    client.loop_forever()
except Exception as e:
    print("Connection error:", e)

# Clean disconnection (optional)
# client.disconnect()