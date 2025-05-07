@Composable
fun VentiladorScreen(onCommandSend: (String) -> Unit) {
    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(24.dp),
        verticalArrangement = Arrangement.spacedBy(16.dp, Alignment.CenterVertically),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Text("Control de Ventilador", style = MaterialTheme.typography.headlineMedium)
        Button(onClick = { onCommandSend("1") }) { Text("Velocidad 1") }
        Button(onClick = { onCommandSend("2") }) { Text("Velocidad 2") }
        Button(onClick = { onCommandSend("3") }) { Text("Velocidad 3") }
        Button(
            onClick = { onCommandSend("0") },
            colors = ButtonDefaults.buttonColors(MaterialTheme.colorScheme.error)
        ) { Text("Apagar") }
    }
}
