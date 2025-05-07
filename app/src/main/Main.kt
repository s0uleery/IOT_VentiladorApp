class MainActivity : ComponentActivity() {
    lateinit var mqttHandler: MqttHandler

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
