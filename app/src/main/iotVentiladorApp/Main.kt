import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import com.s0uleery.iotventiladorapp.mqtt.MqttHandler
import com.s0uleery.iotventiladorapp.ui.VentiladorScreen

class MainActivity : ComponentActivity() {
    private lateinit var mqttHandler: MqttHandler

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mqttHandler = MqttHandler(this)

        setContent {
            VentiladorScreen(
                onCommandSend = { velocidad -> mqttHandler.publishVelocidad(velocidad) }
            )
        }
    }
}
