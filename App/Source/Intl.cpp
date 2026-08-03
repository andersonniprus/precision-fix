#include "Stdafx.hpp"
#include "Intl.hpp"

namespace
{
	struct CatalogEntry
	{
		std::string_view key;
		std::string_view es;
		std::string_view pt_br;
	};

	constexpr CatalogEntry kCatalog[ ] = {
		{ "3D Viewer", "Visor 3D", "Visualizador 3D" },
		{ "A new version is available on GitHub.", "Hay una nueva versión disponible en GitHub.", "Uma nova versão está disponível no GitHub." },
		{ "AMD power gating disabled", "Power gating AMD desactivado", "Power gating AMD desativado" },
		{ "About", "Acerca de", "Sobre" },
		{ "Access denied (requires administrator)", "Acceso denegado (requiere administrador)", "Acesso negado (requer administrador)" },
		{ "Accessibility", "Accesibilidad", "Acessibilidade" },
		{ "Activity feed", "Feed de actividad", "Feed de atividade" },
		{ "Adapter", "Adaptador", "Adaptador" },
		{
			"Adds timestamp options to TCP segments. Off can slightly reduce per-packet overhead.",
			"Añade opciones de marca de tiempo a segmentos TCP. Desactivado puede reducir ligeramente la sobrecarga por paquete.",
			"Adiciona opções de carimbo de tempo a segmentos TCP. Desligado pode reduzir levemente o overhead por pacote."
		},
		{ "Ads, suggestions and sync.", "Anuncios, sugerencias y sincronización.", "Anúncios, sugestões e sincronização." },
		{ "Advanced", "Avanzado", "Avançado" },
		{ "Advertising ID", "ID de publicidad", "ID de publicidade" },
		{ "Alarms & Clock", "Alarmas y reloj", "Alarmes e relógio" },
		{
			"Allows UWP/Store apps to run and refresh in the background.", "Permite que apps UWP/Store se ejecuten y actualicen en segundo plano.",
			"Permite que apps UWP/Store executem e atualizem em segundo plano."
		},
		{
			"Allows apps to show notification banners and the notification center badge.",
			"Permite que las apps muestren banners y la insignia del centro de notificaciones.",
			"Permite que os apps mostrem banners e o distintivo do centro de notificações."
		},
		{
			"Allows apps to take exclusive control of the endpoint (lower latency, no shared mixing).",
			"Permite que las apps tomen control exclusivo del endpoint (menor latencia, sin mezcla compartida).",
			"Permite que os apps tomem controle exclusivo do endpoint (menor latência, sem mix compartilhado)."
		},
		{
			"Allows the CPU to enter low-power idle states (C-states) when unused.",
			"Permite que la CPU entre en estados de inactividad de bajo consumo (C-states) cuando no se usa.",
			"Permite que a CPU entre em estados ociosos de baixo consumo (C-states) quando ociosa."
		},
		{ "Appearance", "Apariencia", "Aparência" },
		{ "Application information and licensing.", "Información de la aplicación y licencia.", "Informações do aplicativo e licença." },
		{ "Application inventory collection", "Inventario de aplicaciones", "Inventário de aplicativos" },
		{
			"Applies Windows' acceleration curve based on movement speed.",
			"Aplica la curva de aceleración de Windows según la velocidad del movimiento.",
			"Aplica a curva de aceleração do Windows com base na velocidade do movimento."
		},
		{ "Apps", "Apps", "Apps" },
		{ "Audio", "Audio", "Áudio" },
		{ "Audio enhancements", "Mejoras de audio", "Aprimoramentos de áudio" },
		{ "Author", "Autor", "Autor" },
		{ "Background apps", "Apps en segundo plano", "Apps em segundo plano" },
		{
			"Background file indexing service (WSearch). Off frees disk and CPU on desktop gaming PCs.",
			"Servicio de indexación de archivos en segundo plano (WSearch). Desactivado libera disco y CPU en PCs de escritorio para juegos.",
			"Serviço de indexação de arquivos em segundo plano (WSearch). Desligado libera disco e CPU em PCs desktop para jogos."
		},
		{
			"Background service that estimates per-app battery usage.", "Servicio en segundo plano que estima el uso de batería por app.",
			"Serviço em segundo plano que estima o uso de bateria por app."
		},
		{
			"Background study of sleep-wake behavior used for diagnostics telemetry.",
			"Estudio en segundo plano del comportamiento de suspensión/reactivación usado para telemetría de diagnóstico.",
			"Estudo em segundo plano do comportamento sono/despertar usado para telemetria de diagnóstico."
		},
		{
			"Background work that competes with games and interactive apps.", "Trabajo en segundo plano que compite con juegos y apps interactivas.",
			"Trabalho em segundo plano que compete com jogos e apps interativos."
		},
		{ "Bing News / Finance / Sports", "Bing Noticias / Finanzas / Deportes", "Bing Notícias / Finanças / Esportes" },
		{
			"Buffered input events the driver keeps before reporting movement.",
			"Eventos de entrada en búfer que el controlador conserva antes de informar el movimiento.",
			"Eventos de entrada em buffer que o driver mantém antes de reportar o movimento."
		},
		{
			"Built-in Windows apps you can remove.", "Apps integradas de Windows que puede eliminar.",
			"Apps integrados do Windows que você pode remover."
		},
		{
			"Bundles small outgoing packets to save bandwidth at the cost of latency.",
			"Agrupa paquetes salientes pequeños para ahorrar ancho de banda a costa de latencia.",
			"Agrupa pacotes de saída pequenos para economizar banda à custa de latência."
		},
		{ "CPU time and multimedia throttling.", "Tiempo de CPU y limitación multimedia.", "Tempo de CPU e limitação multimídia." },
		{
			"CPU transactional memory extension. Recommended on for Intel, off for AMD.",
			"Extensión de memoria transaccional de CPU. Recomendado activado en Intel, desactivado en AMD.",
			"Extensão de memória transacional da CPU. Recomendado ligado na Intel, desligado na AMD."
		},
		{ "CTCP congestion provider", "Proveedor de congestión CTCP", "Provedor de congestionamento CTCP" },
		{ "Camera", "Cámara", "Câmera" },
		{ "Cancel = exit", "Cancelar = salir", "Cancelar = sair" },
		{
			"Caps network throughput the OS reserves for background multimedia tasks.",
			"Limita el rendimiento de red que el SO reserva para tareas multimedia en segundo plano.",
			"Limita a taxa de rede que o SO reserva para tarefas multimídia em segundo plano."
		},
		{
			"Content-protection handshake for the display output. NVIDIA only.",
			"Handshake de protección de contenido para la salida de pantalla. Solo NVIDIA.",
			"Handshake de proteção de conteúdo para a saída de vídeo. Somente NVIDIA."
		},
		{ "Copilot", "Copilot", "Copilot" },
		{ "Cortana", "Cortana", "Cortana" },
		{ "DNS over HTTPS (auto)", "DNS sobre HTTPS (auto)", "DNS over HTTPS (auto)" },
		{ "Dark", "Oscuro", "Escuro" },
		{ "Dark mode", "Modo oscuro", "Modo escuro" },
		{ "Debloat", "Debloat", "Debloat" },
		{ "Default", "Predeterminado", "Padrão" },
		{
			"Delay before a background app is allowed to steal focus from you.", "Retraso antes de que una app en segundo plano pueda robar el foco.",
			"Atraso antes que um app em segundo plano possa roubar o foco."
		},
		{
			"Delay before hovering a menu item opens its submenu.", "Retraso antes de que al pasar el cursor se abra el submenú.",
			"Atraso antes de o submenu abrir ao passar o mouse."
		},
		{ "Delivery Optimization", "Optimización de distribución", "Otimização de entrega" },
		{
			"Desktop Window Manager composition paths.", "Rutas de composición del Administrador de ventanas de escritorio.",
			"Caminhos de composição do Desktop Window Manager."
		},
		{ "Diagnostic data and feedback.", "Datos de diagnóstico y comentarios.", "Dados de diagnóstico e feedback." },
		{ "Diagnostic data collection", "Recopilación de datos de diagnóstico", "Coleta de dados de diagnóstico" },
		{ "Diagnostic scheduled tasks", "Tareas programadas de diagnóstico", "Tarefas agendadas de diagnóstico" },
		{ "Disable 8.3 short filenames", "Desactivar nombres cortos 8.3", "Desativar nomes curtos 8.3" },
		{ "Disable IPv6", "Desactivar IPv6", "Desativar IPv6" },
		{ "Disable NTFS last-access timestamps", "Desactivar marcas de último acceso NTFS", "Desativar timestamps de último acesso NTFS" },
		{
			"Disables AMD driver-level power gating for lower latency at higher idle power. Undocumented vendor tweak.",
			"Desactiva el power gating a nivel de controlador AMD para menor latencia con mayor consumo en reposo. Ajuste de proveedor no documentado.",
			"Desativa o power gating em nível de driver AMD para menor latência com maior consumo em idle. Ajuste de fabricante não documentado."
		},
		{
			"Disables IPv6 on all interfaces and tunnels system-wide.", "Desactiva IPv6 en todas las interfaces y túneles del sistema.",
			"Desativa IPv6 em todas as interfaces e túneis do sistema."
		},
		{
			"Disables NVIDIA driver-level task preemption. Undocumented vendor tweak.",
			"Desactiva la expropiación de tareas a nivel de controlador NVIDIA. Ajuste de proveedor no documentado.",
			"Desativa a preempção de tarefas em nível de driver NVIDIA. Ajuste de fabricante não documentado."
		},
		{
			"Disables Xbox Game Bar recording, capture and overlay for all apps.",
			"Desactiva la grabación, captura y superposición de Xbox Game Bar para todas las apps.",
			"Desativa gravação, captura e overlay do Xbox Game Bar para todos os apps."
		},
		{
			"Disables hardware checksum/segmentation offloading and tunes RSS and buffer sizes for lower latency.",
			"Desactiva la descarga de checksum/segmentación por hardware y ajusta RSS y tamaños de búfer para menor latencia.",
			"Desativa o offload de checksum/segmentação por hardware e ajusta RSS e tamanhos de buffer para menor latência."
		},
		{
			"Disables system-wide location, sensors and the Windows location provider.",
			"Desactiva la ubicación del sistema, sensores y el proveedor de ubicación de Windows.",
			"Desativa localização do sistema, sensores e o provedor de localização do Windows."
		},
		{
			"Disables the Widgets board content feed.", "Desactiva el feed de contenido del panel de Widgets.",
			"Desativa o feed de conteúdo do painel de Widgets."
		},
		{
			"Disables the background scheduled tasks that gather usage and diagnostic data.",
			"Desactiva las tareas programadas en segundo plano que recopilan datos de uso y diagnóstico.",
			"Desativa as tarefas agendadas em segundo plano que coletam dados de uso e diagnóstico."
		},
		{
			"Disables the taskbar News and interests widget content policy.",
			"Desactiva la política de contenido del widget Noticias e intereses de la barra de tareas.",
			"Desativa a política de conteúdo do widget Notícias e interesses da barra de tarefas."
		},
		{ "Display", "Pantalla", "Tela" },
		{ "Drawboard PDF", "Drawboard PDF", "Drawboard PDF" },
		{
			"Driver-level tweaks. Effect not guaranteed on all versions.",
			"Ajustes a nivel de controlador. El efecto no está garantizado en todas las versiones.",
			"Ajustes em nível de driver. O efeito não é garantido em todas as versões."
		},
		{
			"Dynamically grows the receive window for high throughput. Off can cut bufferbloat and jitter (may lower Mbps).",
			"Aumenta dinámicamente la ventana de recepción para alto rendimiento. Desactivado puede reducir bufferbloat y jitter (puede bajar Mbps).",
			"Aumenta dinamicamente a janela de recebimento para alto throughput. Desligado pode reduzir bufferbloat e jitter (pode baixar Mbps)."
		},
		{ "ECN capability", "Capacidad ECN", "Capacidade ECN" },
		{ "Energy estimation engine", "Motor de estimación de energía", "Motor de estimativa de energia" },
		{ "English", "Inglés", "Inglês" },
		{ "Enhance pointer precision", "Mejorar precisión del puntero", "Melhorar precisão do ponteiro" },
		{ "Exclusive mode", "Modo exclusivo", "Modo exclusivo" },
		{
			"Explicit Congestion Notification. Helps routers signal congestion without dropping packets.",
			"Notificación explícita de congestión. Ayuda a los enrutadores a señalar congestión sin descartar paquetes.",
			"Notificação explícita de congestionamento. Ajuda roteadores a sinalizar congestão sem descartar pacotes."
		},
		{ "Explorer", "Explorador", "Explorador" },
		{ "Fast DNS (Cloudflare)", "DNS rápido (Cloudflare)", "DNS rápido (Cloudflare)" },
		{ "Fast TCP port recycling", "Reciclaje rápido de puertos TCP", "Reciclagem rápida de portas TCP" },
		{ "Fast app termination", "Terminación rápida de apps", "Encerramento rápido de apps" },
		{ "Fast name resolution", "Resolución de nombres rápida", "Resolução de nomes rápida" },
		{ "Fast startup", "Inicio rápido", "Inicialização rápida" },
		{ "Fastest", "Más rápido", "Mais rápido" },
		{ "Feedback Hub", "Centro de opiniones", "Hub de Feedback" },
		{ "Feedback prompts", "Solicitudes de comentarios", "Solicitações de feedback" },
		{ "Filter Keys", "Teclas de filtro", "Teclas de filtro" },
		{ "Focus, menus and UI latency.", "Enfoque, menús y latencia de la interfaz.", "Foco, menus e latência da interface." },
		{
			"Forces GPU/display power-management latency tolerances to their lowest values. Undocumented vendor tweak.",
			"Fuerza las tolerancias de latencia de administración de energía de GPU/pantalla a sus valores más bajos. Ajuste no documentado.",
			"Força as tolerâncias de latência de gerenciamento de energia de GPU/tela aos valores mais baixos. Ajuste não documentado."
		},
		{ "Foreground lock timeout", "Tiempo de bloqueo en primer plano", "Timeout de bloqueio em primeiro plano" },
		{ "GPU", "GPU", "GPU" },
		{ "GPU preemption", "Expropiación de GPU", "Preempção de GPU" },
		{ "GPU task scheduling and preemption.", "Programación y expropiación de tareas de GPU.", "Agendamento e preempção de tarefas da GPU." },
		{ "Game Bar / Game DVR", "Barra de juegos / Game DVR", "Barra de jogos / Game DVR" },
		{ "Game Bar and location services.", "Barra de juegos y servicios de ubicación.", "Barra de jogos e serviços de localização." },
		{ "Game Mode", "Modo de juego", "Modo de jogo" },
		{ "Games MMCSS profile", "Perfil MMCSS de juegos", "Perfil MMCSS de jogos" },
		{ "Gaming & location", "Juegos y ubicación", "Jogos e localização" },
		{ "Get Help", "Obtener ayuda", "Obter ajuda" },
		{ "GitHub", "GitHub", "GitHub" },
		{ "Graphics latency tolerance override", "Anulación de tolerancia de latencia gráfica", "Substituição de tolerância de latência gráfica" },
		{
			"Groups timer interrupts together to reduce CPU wake-ups.", "Agrupa interrupciones de temporizador para reducir despertares de la CPU.",
			"Agrupa interrupções de timer para reduzir despertar da CPU."
		},
		{ "HDCP", "HDCP", "HDCP" },
		{ "HEIF Image Extension", "Extensión de imagen HEIF", "Extensão de imagem HEIF" },
		{
			"Hardware overlay planes for the desktop. Off can fix stutter on some multi-monitor / VRR setups.",
			"Planos de superposición de hardware para el escritorio. Desactivado puede corregir stutter en algunas configuraciones multi-monitor / VRR.",
			"Planos de overlay de hardware para a área de trabalho. Desligado pode corrigir stutter em algumas configurações multi-monitor / VRR."
		},
		{ "Hardware-accelerated GPU scheduling", "Programación de GPU acelerada por hardware", "Agendamento de GPU acelerado por hardware" },
		{ "Hibernate", "Hibernar", "Hibernar" },
		{
			"Hibernates the kernel session on shutdown to speed up the next boot.",
			"Hiberna la sesión del kernel al apagar para acelerar el próximo arranque.",
			"Hiberna a sessão do kernel no desligamento para acelerar o próximo boot."
		},
		{ "High", "Alto", "Alto" },
		{
			"Horizontal acceleration curve. Optimized removes added acceleration.",
			"Curva de aceleración horizontal. Optimizado elimina la aceleración añadida.",
			"Curva de aceleração horizontal. Otimizado remove a aceleração adicionada."
		},
		{ "Ignores brief or repeated keystrokes.", "Ignora pulsaciones breves o repetidas.", "Ignora teclas breves ou repetidas." },
		{ "Info", "Info", "Info" },
		{ "Input buffering and queue size.", "Búfer de entrada y tamaño de cola.", "Buffer de entrada e tamanho da fila." },
		{ "Intel TSX", "Intel TSX", "Intel TSX" },
		{ "Internet connectivity probing", "Sondeo de conectividad a Internet", "Sondagem de conectividade com a Internet" },
		{ "Invalid value", "Valor no válido", "Valor inválido" },
		{ "Key repeat delay and rate.", "Retardo y velocidad de repetición de teclas.", "Atraso e taxa de repetição de teclas." },
		{ "Keyboard", "Teclado", "Teclado" },
		{ "Language", "Idioma", "Idioma" },
		{ "Latency", "Latencia", "Latência" },
		{
			"Legacy QoS reservation that can cap throughput. Off sets NonBestEffortLimit to 0.",
			"Reserva QoS heredada que puede limitar el rendimiento. Desactivado pone NonBestEffortLimit en 0.",
			"Reserva QoS legada que pode limitar o throughput. Desligado define NonBestEffortLimit como 0."
		},
		{
			"Lets Windows automatically use known DoH servers for encrypted name resolution.",
			"Permite que Windows use automáticamente servidores DoH conocidos para resolución de nombres cifrada.",
			"Permite que o Windows use automaticamente servidores DoH conhecidos para resolução de nomes criptografada."
		},
		{
			"Lets Windows downclock the CPU to save energy under light load.",
			"Permite que Windows reduzca la frecuencia de la CPU para ahorrar energía con carga ligera.",
			"Permite que o Windows reduza o clock da CPU para economizar energia sob carga leve."
		},
		{
			"Lets Windows prioritize the foreground game and reduce background work while gaming.",
			"Permite que Windows priorice el juego en primer plano y reduzca el trabajo en segundo plano al jugar.",
			"Permite que o Windows priorize o jogo em primeiro plano e reduza o trabalho em segundo plano ao jogar."
		},
		{
			"Lets Windows size the page file automatically. Never disable the page file; On restores the safe default.",
			"Permite que Windows dimension el archivo de paginación automáticamente. Nunca desactive el archivo de paginación; Activado restaura el valor seguro.",
			"Permite que o Windows dimensione o arquivo de paginação automaticamente. Nunca desative o arquivo de paginação; Ligado restaura o padrão seguro."
		},
		{
			"Lets Windows suspend idle USB devices to save power.", "Permite que Windows suspenda dispositivos USB inactivos para ahorrar energía.",
			"Permite que o Windows suspenda dispositivos USB ociosos para economizar energia."
		},
		{
			"Lets modifier keys stay active without holding them down.",
			"Permite que las teclas modificadoras sigan activas sin mantenerlas pulsadas.",
			"Permite que teclas modificadoras fiquem ativas sem mantê-las pressionadas."
		},
		{
			"Lets the GPU manage its own video memory queue instead of the OS scheduler.",
			"Permite que la GPU administre su propia cola de memoria de video en lugar del programador del SO.",
			"Permite que a GPU gerencie sua própria fila de memória de vídeo em vez do agendador do SO."
		},
		{
			"Lets the numeric keypad move the mouse pointer.", "Permite que el teclado numérico mueva el puntero del ratón.",
			"Permite que o teclado numérico mova o ponteiro do mouse."
		},
		{
			"Lets the scheduler interrupt a running graphics task to service another one.",
			"Permite que el programador interrumpa una tarea gráfica en ejecución para atender otra.",
			"Permite que o agendador interrompa uma tarefa gráfica em execução para atender outra."
		},
		{ "Light", "Claro", "Claro" },
		{ "Linux-like TTL", "TTL tipo Linux", "TTL estilo Linux" },
		{ "Location services", "Servicios de ubicación", "Serviços de localização" },
		{ "Logs", "Registros", "Logs" },
		{ "Low", "Bajo", "Baixo" },
		{
			"Low-power connected standby (S0). Off forces classic S3 sleep on supported hardware.",
			"Espera conectada de bajo consumo (S0). Desactivado fuerza el sueño clásico S3 en hardware compatible.",
			"Espera conectada de baixo consumo (S0). Desligado força o sono clássico S3 em hardware suportado."
		},
		{ "Mail and Calendar", "Correo y Calendario", "Email e Calendário" },
		{ "Maps", "Mapas", "Mapas" },
		{ "Media Player (Groove Music)", "Reproductor multimedia (Groove Music)", "Media Player (Groove Music)" },
		{ "Medium", "Medio", "Médio" },
		{ "Menu show delay", "Retardo de menús", "Atraso de exibição de menus" },
		{
			"Menu, tooltip and cursor shadow animations. Off trims perceived click latency.",
			"Animaciones de menú, tooltip y sombra del cursor. Desactivado reduce la latencia percibida del clic.",
			"Animações de menu, tooltip e sombra do cursor. Desligado reduz a latência percebida do clique."
		},
		{ "Messaging", "Mensajería", "Mensagens" },
		{
			"Minimum minutes between automatic System Restore checkpoints. Optimized creates one on every change.",
			"Minutos mínimos entre puntos automáticos de Restaurar sistema. Optimizado crea uno en cada cambio.",
			"Minutos mínimos entre pontos automáticos de Restauração do Sistema. Otimizado cria um a cada mudança."
		},
		{ "Mixed Reality Portal", "Portal de realidad mixta", "Portal de realidade mista" },
		{ "Mobile Plans", "Planes móviles", "Planos móveis" },
		{ "Modern Standby", "Modern Standby", "Modern Standby" },
		{ "Mouse", "Ratón", "Mouse" },
		{ "Mouse Keys", "Teclas de ratón", "Mouse Keys" },
		{ "Mouse data queue size", "Tamaño de cola de datos del ratón", "Tamanho da fila de dados do mouse" },
		{ "Multi-plane overlays (MPO)", "Superposiciones multiplano (MPO)", "Overlays multiplano (MPO)" },
		{ "NIC offload tuning", "Ajuste de offload de NIC", "Ajuste de offload da NIC" },
		{ "NIC power and offload tuning.", "Ajuste de energía y offload de NIC.", "Ajuste de energia e offload da NIC." },
		{ "NIC power saving", "Ahorro de energía de NIC", "Economia de energia da NIC" },
		{ "NTFS and service host layout.", "NTFS y diseño de hosts de servicio.", "NTFS e layout de hosts de serviço." },
		{ "NVIDIA preemption override", "Anulación de expropiación NVIDIA", "Substituição de preempção NVIDIA" },
		{ "Nagle's algorithm", "Algoritmo de Nagle", "Algoritmo de Nagle" },
		{ "Network", "Red", "Rede" },
		{ "Network throttling index", "Índice de limitación de red", "Índice de limitação de rede" },
		{ "News and interests", "Noticias e intereses", "Notícias e interesses" },
		{ "No = open GitHub and exit", "No = abrir GitHub y salir", "Não = abrir GitHub e sair" },
		{ "No logs yet.", "Aún no hay registros.", "Ainda não há logs." },
		{ "Not implemented", "No implementado", "Não implementado" },
		{ "Not supported on this system", "No compatible con este sistema", "Não suportado neste sistema" },
		{ "Off", "Desactivado", "Desligado" },
		{ "On", "Activado", "Ligado" },
		{ "Open GitHub", "Abrir GitHub", "Abrir GitHub" },
		{ "Open source — not authorized for sale", "Código abierto — no autorizado para la venta", "Código aberto — não autorizado para venda" },
		{ "Optimized", "Optimizado", "Otimizado" },
		{
			"Overrides every adapter's DNS servers with 1.1.1.1 / 1.0.0.1.", "Reemplaza los servidores DNS de cada adaptador con 1.1.1.1 / 1.0.0.1.",
			"Substitui os servidores DNS de cada adaptador por 1.1.1.1 / 1.0.0.1."
		},
		{
			"Peer-to-peer Windows Update / Store downloads. Off limits sharing and background bandwidth use.",
			"Descargas P2P de Windows Update / Store. Desactivado limita el uso compartido y el ancho de banda en segundo plano.",
			"Downloads P2P do Windows Update / Store. Desligado limita o compartilhamento e o uso de banda em segundo plano."
		},
		{ "People", "Personas", "Pessoas" },
		{
			"Per-user identifier apps use to personalize and track advertising.",
			"Identificador por usuario que las apps usan para personalizar y rastrear publicidad.",
			"Identificador por usuário que os apps usam para personalizar e rastrear publicidade."
		},
		{
			"Percentage of CPU reserved away from background multimedia tasks.",
			"Porcentaje de CPU reservado fuera de tareas multimedia en segundo plano.",
			"Porcentagem de CPU reservada fora de tarefas multimídia em segundo plano."
		},
		{ "Performance gain", "Ganancia de rendimiento", "Ganho de desempenho" },
		{
			"Periodically probes a Microsoft endpoint in the background to confirm connectivity.",
			"Sondea periódicamente un endpoint de Microsoft en segundo plano para confirmar conectividad.",
			"Sonda periodicamente um endpoint da Microsoft em segundo plano para confirmar conectividade."
		},
		{ "Personalization", "Personalización", "Personalização" },
		{ "Phone Link", "Enlace al teléfono", "Link com o celular" },
		{ "Playback", "Reproducción", "Reprodução" },
		{
			"Plays a tone when Caps Lock, Num Lock or Scroll Lock changes.", "Reproduce un tono cuando cambian Caps Lock, Num Lock o Scroll Lock.",
			"Toca um tom quando Caps Lock, Num Lock ou Scroll Lock mudam."
		},
		{ "Pointer acceleration and speed.", "Aceleración y velocidad del puntero.", "Aceleração e velocidade do ponteiro." },
		{ "Pointer speed", "Velocidad del puntero", "Velocidade do ponteiro" },
		{ "Portuguese (Brazil)", "Portugués (Brasil)", "Português (Brasil)" },
		{ "Power", "Energía", "Energia" },
		{ "Power throttling", "Limitación de energía", "Limitação de energia" },
		{ "Precision", "Precisión", "Precisão" },
		{ "Prefetch", "Prefetch", "Prefetch" },
		{ "Prefetch and CPU extensions.", "Prefetch y extensiones de CPU.", "Prefetch e extensões de CPU." },
		{
			"Prefetches apps into RAM in the background. Often less useful on fast SSDs.",
			"Precarga apps en RAM en segundo plano. Suele ser menos útil en SSD rápidos.",
			"Pré-carrega apps na RAM em segundo plano. Costuma ser menos útil em SSDs rápidos."
		},
		{
			"Preloads frequently used applications into memory ahead of time.", "Precarga en memoria aplicaciones de uso frecuente.",
			"Pré-carrega na memória aplicativos usados com frequência."
		},
		{ "Print 3D", "Imprimir 3D", "Impressão 3D" },
		{ "Privacy", "Privacidad", "Privacidade" },
		{ "Processor idle states", "Estados de inactividad del procesador", "Estados ociosos do processador" },
		{ "QoS non-best-effort bandwidth reserve", "Reserva de ancho de banda QoS non-best-effort", "Reserva de banda QoS non-best-effort" },
		{
			"RAM, in KB, above which Windows splits shared services into separate svchost.exe processes.",
			"RAM, en KB, por encima de la cual Windows divide servicios compartidos en procesos svchost.exe separados.",
			"RAM, em KB, acima da qual o Windows divide serviços compartilhados em processos svchost.exe separados."
		},
		{
			"Raises GPU/CPU priority for the multimedia Games task (GPU Priority 8, Priority 6, High scheduling).",
			"Eleva la prioridad GPU/CPU de la tarea multimedia Games (GPU Priority 8, Priority 6, High).",
			"Eleva a prioridade GPU/CPU da tarefa multimídia Games (GPU Priority 8, Priority 6, High)."
		},
		{
			"Raises the maximum ephemeral outgoing port to 65534 for more concurrent connections.",
			"Eleva el puerto efímero saliente máximo a 65534 para más conexiones concurrentes.",
			"Eleva a porta efêmera de saída máxima para 65534 para mais conexões simultâneas."
		},
		{ "Receive-side scaling (RSS)", "Escalado del lado de recepción (RSS)", "Receive-side scaling (RSS)" },
		{ "Recommended", "Recomendado", "Recomendado" },
		{
			"Removes every built-in Bing-branded app.", "Elimina todas las apps integradas de marca Bing.",
			"Remove todos os apps integrados da marca Bing."
		},
		{ "Removes the 3D Viewer app.", "Elimina la app Visor 3D.", "Remove o app Visualizador 3D." },
		{ "Removes the Alarms & Clock app.", "Elimina la app Alarmas y reloj.", "Remove o app Alarmes e relógio." },
		{ "Removes the Cortana app.", "Elimina la app Cortana.", "Remove o app Cortana." },
		{ "Removes the Drawboard PDF annotation app.", "Elimina la app de anotación Drawboard PDF.", "Remove o app de anotações Drawboard PDF." },
		{ "Removes the Feedback Hub app.", "Elimina la app Centro de opiniones.", "Remove o app Hub de Feedback." },
		{ "Removes the Get Help support app.", "Elimina la app de soporte Obtener ayuda.", "Remove o app de suporte Obter ajuda." },
		{ "Removes the HEIF image codec extension.", "Elimina la extensión de códec de imagen HEIF.", "Remove a extensão de codec de imagem HEIF." },
		{
			"Removes the Microsoft Solitaire Collection game.", "Elimina el juego Microsoft Solitaire Collection.",
			"Remove o jogo Microsoft Solitaire Collection."
		},
		{ "Removes the Microsoft Sway app.", "Elimina la app Microsoft Sway.", "Remove o app Microsoft Sway." },
		{ "Removes the Mobile Plans (OneConnect) app.", "Elimina la app Planes móviles (OneConnect).", "Remove o app Planos móveis (OneConnect)." },
		{ "Removes the People contacts app.", "Elimina la app de contactos Personas.", "Remove o app de contatos Pessoas." },
		{
			"Removes the Phone Link (Your Phone) companion app.", "Elimina la app compañera Enlace al teléfono (Tu teléfono).",
			"Remove o app companheiro Link com o celular (Seu telefone)."
		},
		{ "Removes the Print 3D app.", "Elimina la app Imprimir 3D.", "Remove o app Impressão 3D." },
		{ "Removes the Tips (Get Started) app.", "Elimina la app Consejos (Empezar).", "Remove o app Dicas (Introdução)." },
		{ "Removes the Voice Recorder app.", "Elimina la app Grabadora de voz.", "Remove o app Gravador de voz." },
		{
			"Removes the Web Media Extensions codec pack.", "Elimina el paquete de códecs Web Media Extensions.",
			"Remove o pacote de codecs Web Media Extensions."
		},
		{ "Removes the WebP image codec extension.", "Elimina la extensión de códec de imagen WebP.", "Remove a extensão de codec de imagem WebP." },
		{
			"Removes the Windows Mixed Reality Portal.", "Elimina el Portal de realidad mixta de Windows.",
			"Remove o Portal de realidade mista do Windows."
		},
		{ "Removes the built-in Camera app.", "Elimina la app Cámara integrada.", "Remove o app Câmera integrado." },
		{
			"Removes the built-in Mail and Calendar apps.", "Elimina las apps integradas Correo y Calendario.",
			"Remove os apps integrados Email e Calendário."
		},
		{
			"Removes the built-in Maps app and offline map data.", "Elimina la app Mapas integrada y los datos de mapas sin conexión.",
			"Remove o app Mapas integrado e dados de mapas offline."
		},
		{ "Removes the built-in Skype app.", "Elimina la app Skype integrada.", "Remove o app Skype integrado." },
		{ "Removes the built-in Sticky Notes app.", "Elimina la app Notas adhesivas integrada.", "Remove o app Sticky Notes integrado." },
		{
			"Removes the built-in Weather app for all users.", "Elimina la app Clima integrada para todos los usuarios.",
			"Remove o app Clima integrado para todos os usuários."
		},
		{ "Removes the built-in Windows Copilot app.", "Elimina la app Windows Copilot integrada.", "Remove o app Windows Copilot integrado." },
		{ "Removes the legacy Groove Music app.", "Elimina la app heredada Groove Music.", "Remove o app legado Groove Music." },
		{ "Removes the legacy Messaging app.", "Elimina la app heredada de mensajería.", "Remove o app legado de mensagens." },
		{ "Repeat", "Repetición", "Repetição" },
		{ "Repeat delay", "Retardo de repetición", "Atraso de repetição" },
		{ "Repeat rate", "Velocidad de repetición", "Taxa de repetição" },
		{
			"Reprioritizes the DNS/hosts/NetBIOS name-resolution provider order.",
			"Reordena la prioridad de proveedores de resolución de nombres DNS/hosts/NetBIOS.",
			"Reordena a prioridade dos provedores de resolução de nomes DNS/hosts/NetBIOS."
		},
		{ "Responsiveness", "Respuesta", "Responsividade" },
		{ "Restore point frequency", "Frecuencia de puntos de restauración", "Frequência de pontos de restauração" },
		{
			"Runtime events from features and the application.", "Eventos en tiempo de ejecución de funciones y la aplicación.",
			"Eventos em tempo de execução das funções e do aplicativo."
		},
		{
			"Saves the current session to disk and powers off completely.", "Guarda la sesión actual en disco y apaga por completo.",
			"Salva a sessão atual em disco e desliga por completo."
		},
		{ "Scheduling", "Programación", "Agendamento" },
		{ "Services & background", "Servicios y segundo plano", "Serviços e segundo plano" },
		{
			"Sets the default IP time-to-live to 64, matching Linux/macOS defaults.",
			"Establece el TTL IP predeterminado en 64, igual que Linux/macOS.", "Define o TTL IP padrão como 64, igual ao Linux/macOS."
		},
		{ "Settings sync", "Sincronización de configuración", "Sincronização de configurações" },
		{ "Shell appearance and notifications.", "Apariencia del shell y notificaciones.", "Aparência do shell e notificações." },
		{
			"Shortens the TIME_WAIT delay before a closed TCP port can be reused.",
			"Acorta el retraso TIME_WAIT antes de reutilizar un puerto TCP cerrado.",
			"Encurta o atraso TIME_WAIT antes de reutilizar uma porta TCP fechada."
		},
		{
			"Shortens the delay before Windows force-closes a hung or hooked application.",
			"Acorta el retraso antes de que Windows cierre a la fuerza una aplicación colgada o enganchada.",
			"Encurta o atraso antes de o Windows forçar o fechamento de um aplicativo travado ou hookado."
		},
		{ "Show file extensions", "Mostrar extensiones de archivo", "Mostrar extensões de arquivo" },
		{
			"Shows the file name extension for every file in Explorer.",
			"Muestra la extensión del nombre de archivo para cada archivo en el Explorador.",
			"Mostra a extensão do nome de arquivo para cada arquivo no Explorador."
		},
		{ "Skype", "Skype", "Skype" },
		{ "Sleep diagnostics", "Diagnósticos de suspensión", "Diagnósticos de suspensão" },
		{ "Sleep, standby and energy policies.", "Políticas de suspensión, espera y energía.", "Políticas de suspensão, espera e energia." },
		{ "Slow", "Lento", "Lento" },
		{ "Smooth mouse X curve", "Curva suave del ratón X", "Curva suave do mouse X" },
		{ "Smooth mouse Y curve", "Curva suave del ratón Y", "Curva suave do mouse Y" },
		{ "Solitaire Collection", "Colección de solitarios", "Coleção de Paciência" },
		{ "Spanish", "Español", "Espanhol" },
		{
			"Speed of key repetition once it starts.", "Velocidad de repetición de teclas una vez iniciada.",
			"Velocidade de repetição das teclas após iniciar."
		},
		{
			"Splits CPU time between the foreground application and background tasks.",
			"Divide el tiempo de CPU entre la aplicación en primer plano y tareas en segundo plano.",
			"Divide o tempo de CPU entre o aplicativo em primeiro plano e tarefas em segundo plano."
		},
		{
			"Spreads inbound packet processing across CPU cores at the TCP stack level.",
			"Distribuye el procesamiento de paquetes entrantes entre núcleos de CPU a nivel de pila TCP.",
			"Distribui o processamento de pacotes de entrada entre núcleos de CPU no nível da pilha TCP."
		},
		{ "Stack", "Pila", "Pilha" },
		{
			"Stack-level latency and bufferbloat controls via netsh.", "Controles de latencia y bufferbloat a nivel de pila mediante netsh.",
			"Controles de latência e bufferbloat em nível de pilha via netsh."
		},
		{ "Sticky Keys", "Teclas sticky", "Sticky Keys" },
		{ "Sticky Notes", "Notas adhesivas", "Sticky Notes" },
		{ "Sticky, filter and mouse-key helpers.", "Ayudas de teclas sticky, filtro y mouse keys.", "Ajudas de sticky keys, filtro e mouse keys." },
		{
			"Stops NTFS from generating legacy DOS-compatible short file names.", "Impide que NTFS genere nombres cortos compatibles con DOS.",
			"Impede que o NTFS gere nomes curtos compatíveis com DOS."
		},
		{
			"Stops Start menu suggestions and Microsoft Store app pre-installs.",
			"Detiene las sugerencias del menú Inicio y las preinstalaciones de apps de Microsoft Store.",
			"Interrompe sugestões do menu Iniciar e pré-instalações de apps da Microsoft Store."
		},
		{
			"Stops Windows from periodically asking for feedback.", "Impide que Windows pida comentarios periódicamente.",
			"Impede que o Windows peça feedback periodicamente."
		},
		{
			"Stops Windows from recording and uploading your activity history.", "Impide que Windows registre y cargue su historial de actividad.",
			"Impede que o Windows registre e envie seu histórico de atividades."
		},
		{
			"Stops Windows from syncing settings, theme and layout to your Microsoft account.",
			"Impide que Windows sincronice configuración, tema y diseño con su cuenta Microsoft.",
			"Impede que o Windows sincronize configurações, tema e layout com sua conta Microsoft."
		},
		{
			"Stops Windows from using your diagnostic data to personalize tips and suggestions.",
			"Impide que Windows use sus datos de diagnóstico para personalizar consejos y sugerencias.",
			"Impede que o Windows use seus dados de diagnóstico para personalizar dicas e sugestões."
		},
		{
			"Stops crash dumps and error reports from being sent to Microsoft.",
			"Impide que se envíen volcados de memoria e informes de error a Microsoft.",
			"Impede que dumps de falha e relatórios de erro sejam enviados à Microsoft."
		},
		{
			"Stops the compatibility appraiser from inventorying installed applications.",
			"Impide que el evaluador de compatibilidad inventarie las aplicaciones instaladas.",
			"Impede que o avaliador de compatibilidade inventarie os aplicativos instalados."
		},
		{
			"Stops updating a file's last-accessed time on every read.", "Deja de actualizar la hora de último acceso de un archivo en cada lectura.",
			"Para de atualizar a hora de último acesso de um arquivo a cada leitura."
		},
		{ "Storage", "Almacenamiento", "Armazenamento" },
		{ "Suggested content & pre-installed apps", "Contenido sugerido y apps preinstaladas", "Conteúdo sugerido e apps pré-instalados" },
		{ "Sway", "Sway", "Sway" },
		{
			"Switches Windows apps to the dark color theme.", "Cambia las apps de Windows al tema de color oscuro.",
			"Alterna os apps do Windows para o tema de cores escuro."
		},
		{ "SysMain (Superfetch)", "SysMain (Superfetch)", "SysMain (Superfetch)" },
		{ "System", "Sistema", "Sistema" },
		{
			"System effects (APOs) applied to every active playback and recording device. Support varies by driver.",
			"Efectos del sistema (APO) aplicados a cada dispositivo de reproducción y grabación activo. El soporte varía según el controlador.",
			"Efeitos do sistema (APOs) aplicados a cada dispositivo de reprodução e gravação ativo. O suporte varia conforme o driver."
		},
		{ "System impact", "Impacto del sistema", "Impacto no sistema" },
		{ "System responsiveness", "Respuesta del sistema", "Responsividade do sistema" },
		{ "System-managed page file", "Archivo de paginación administrado por el sistema", "Arquivo de paginação gerenciado pelo sistema" },
		{ "System-wide audio effects.", "Efectos de audio del sistema.", "Efeitos de áudio do sistema." },
		{ "TCP global", "TCP global", "TCP global" },
		{ "TCP receive window auto-tuning", "Ajuste automático de ventana de recepción TCP", "Autoajuste da janela de recebimento TCP" },
		{ "TCP timestamps (RFC 1323)", "Marcas de tiempo TCP (RFC 1323)", "Carimbos de tempo TCP (RFC 1323)" },
		{ "TCP/IP and DNS behavior.", "Comportamiento de TCP/IP y DNS.", "Comportamento de TCP/IP e DNS." },
		{ "Tailored experiences", "Experiencias personalizadas", "Experiências personalizadas" },
		{ "Telemetry", "Telemetría", "Telemetria" },
		{
			"The main Windows telemetry opt-out (requires an Enterprise/Education edition for full effect).",
			"La exclusión principal de telemetría de Windows (requiere edición Enterprise/Education para efecto completo).",
			"A principal recusa de telemetria do Windows (requer edição Enterprise/Education para efeito completo)."
		},
		{ "Theme", "Tema", "Tema" },
		{ "Theme and language preferences.", "Preferencias de tema e idioma.", "Preferências de tema e idioma." },
		{
			"Time before a held key starts repeating.", "Tiempo antes de que una tecla mantenida empiece a repetirse.",
			"Tempo antes de uma tecla mantida começar a repetir."
		},
		{ "Timer coalescing", "Fusión de temporizadores", "Coalescência de timers" },
		{ "Tips", "Consejos", "Dicas" },
		{ "Toast notifications", "Notificaciones toast", "Notificações toast" },
		{ "Toggle Keys", "Teclas de alternancia", "Teclas de alternância" },
		{
			"Toggles adapter-level Wake-on-LAN and magic-packet wake support.",
			"Activa o desactiva Wake-on-LAN y wake por magic packet a nivel de adaptador.",
			"Ativa ou desativa Wake-on-LAN e wake por magic packet em nível de adaptador."
		},
		{
			"Toggles adapter-level power-saving features (EEE, green Ethernet, link-speed reduction).",
			"Activa o desactiva el ahorro de energía del adaptador (EEE, green Ethernet, reducción de velocidad de enlace).",
			"Ativa ou desativa economia de energia do adaptador (EEE, green Ethernet, redução de velocidade do link)."
		},
		{ "UI animations", "Animaciones de UI", "Animações da UI" },
		{ "USB power management.", "Administración de energía USB.", "Gerenciamento de energia USB." },
		{ "USB selective suspend", "Suspensión selectiva USB", "Suspensão seletiva USB" },
		{ "Unknown error", "Error desconocido", "Erro desconhecido" },
		{ "Update available", "Actualización disponible", "Atualização disponível" },
		{
			"Uses Compound TCP instead of the default congestion control algorithm when supported.",
			"Usa Compound TCP en lugar del algoritmo de control de congestión predeterminado cuando es compatible.",
			"Usa Compound TCP em vez do algoritmo padrão de controle de congestão quando suportado."
		},
		{ "Value not present", "Valor no presente", "Valor não presente" },
		{ "Vendor", "Fabricante", "Fabricante" },
		{ "Version", "Versión", "Versão" },
		{
			"Vertical acceleration curve. Optimized removes added acceleration.",
			"Curva de aceleración vertical. Optimizado elimina la aceleración añadida.",
			"Curva de aceleração vertical. Otimizado remove a aceleração adicionada."
		},
		{ "Voice Recorder", "Grabadora de voz", "Gravador de voz" },
		{ "Wake-on-LAN", "Wake-on-LAN", "Wake-on-LAN" },
		{ "Weather", "Clima", "Clima" },
		{ "Web Media Extensions", "Extensiones multimedia web", "Extensões de mídia da Web" },
		{ "WebP Image Extension", "Extensión de imagen WebP", "Extensão de imagem WebP" },
		{ "Wide dynamic port range", "Rango amplio de puertos dinámicos", "Faixa ampla de portas dinâmicas" },
		{ "Win32 priority separation", "Separación de prioridad Win32", "Separação de prioridade Win32" },
		{ "Windows Error Reporting", "Informe de errores de Windows", "Relatório de erros do Windows" },
		{ "Windows Search indexing", "Indexación de Windows Search", "Indexação do Windows Search" },
		{ "Windows feeds", "Feeds de Windows", "Feeds do Windows" },
		{
			"Windows pointer-speed slider setting, from 1 (slowest) to 20 (fastest).",
			"Ajuste del control deslizante de velocidad del puntero de Windows, de 1 (más lento) a 20 (más rápido).",
			"Ajuste do controle deslizante de velocidade do ponteiro do Windows, de 1 (mais lento) a 20 (mais rápido)."
		},
		{ "Yes = continue", "Sí = continuar", "Sim = continuar" },
		{ "svchost.exe split threshold", "Umbral de división de svchost.exe", "Limiar de divisão do svchost.exe" },
	};

	[[nodiscard]] const CatalogEntry* find_entry( const std::string_view key ) noexcept
	{
		const auto it = std::ranges::lower_bound(
			kCatalog,
			key,
			{},
			&CatalogEntry::key
		);

		if ( it == std::end( kCatalog ) || it->key != key )
			return nullptr;

		return &( *it );
	}
}

namespace App
{
	const char* Intl::tr( const std::string_view key ) const noexcept
	{
		if ( language_ == Language::En )
			return key.data( );

		const auto* entry = find_entry( key );

		if ( !entry )
			return key.data( );

		switch ( language_ )
		{
			case Language::Es:
				return entry->es.data( );

			case Language::PtBr:
				return entry->pt_br.data( );

			default:
				return key.data( );
		}
	}

	std::wstring Intl::tr_w( const std::string_view key ) const
	{
		const char* text = tr( key );
		const int needed = MultiByteToWideChar( CP_UTF8, 0, text, -1, nullptr, 0 );

		if ( needed <= 0 )
			return {};

		std::wstring wide( static_cast<std::size_t>( needed - 1 ), L'\0' );
		MultiByteToWideChar( CP_UTF8, 0, text, -1, wide.data( ), needed );
		return wide;
	}
}
