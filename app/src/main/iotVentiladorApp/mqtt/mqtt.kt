import android.content.Context
import android.util.Log
import org.eclipse.paho.android.service.MqttAndroidClient
import org.eclipse.paho.client.mqttv3.*
import com.s0uleery.iotventiladorapp.utils.Constants

class MqttHandler(context: Context) {

    private val client = MqttAndroidClient(
        context,
        Constants.MQTT_SERVER_URI,
        Constants.MQTT_CLIENT_ID
    )

    init {
        val options = MqttConnectOptions().apply {
            isAutomaticReconnect = true
            isCleanSession = true
        }

        client.connect(options, null, object : IMqttActionListener {
            override fun onSuccess(asyncActionToken: IMqttToken?) {
                Log.d("MQTT", "✅ Conectado a MQTT")
            }

            override fun onFailure(asyncActionToken: IMqttToken?, exception: Throwable?) {
                Log.e("MQTT", "❌ Error al conectar: ${exception?.message}")
            }
        })
    }

    fun publishVelocidad(velocidad: String) {
        val message = MqttMessage(velocidad.toByteArray()).apply { qos = 1 }
        client.publish(Constants.MQTT_TOPIC, message)
    }
}
