import 'dart:ffi' as ffi;
import 'dart:io';
import 'package:ffi/ffi.dart';
import 'models/hev_socks5_config.dart';
import 'models/hev_socks5_stats.dart';

/// Typedef for native HevSocks5Instance
typedef HevSocks5Instance = ffi.Pointer<ffi.Void>;

/// Typedef for log callback
typedef NativeLogCallback =
    ffi.Void Function(
      ffi.Pointer<Utf8> level,
      ffi.Pointer<Utf8> message,
      ffi.Pointer<ffi.Void> userData,
    );

/// Native config struct
final class NativeHevSocks5Config extends ffi.Struct {
  external ffi.Pointer<Utf8> socks5_server;
  external ffi.Pointer<Utf8> tun_name;
  external ffi.Pointer<Utf8> tun_address;
  external ffi.Pointer<Utf8> tun_gateway;
  external ffi.Pointer<Utf8> tun_netmask;
  external ffi.Pointer<Utf8> dns_server;

  @ffi.Uint16()
  external int tun_mtu;

  @ffi.Int32()
  external int tun_fd;

  @ffi.Bool()
  external bool enable_ipv6;

  external ffi.Pointer<Utf8> username;
  external ffi.Pointer<Utf8> password;
}

/// Native stats struct
final class NativeHevSocks5Stats extends ffi.Struct {
  @ffi.Uint64()
  external int tx_packets;

  @ffi.Uint64()
  external int rx_packets;

  @ffi.Uint64()
  external int tx_bytes;

  @ffi.Uint64()
  external int rx_bytes;
}

/// FFI bindings for HevSocks5
class HevSocks5Bindings {
  late final ffi.DynamicLibrary _lib;
  late final _CreateFn _create;
  late final _InitFn _init;
  late final _StartFn _start;
  late final _StopFn _stop;
  late final _IsRunningFn _isRunning;
  late final _GetStatsFn _getStats;
  late final _SetLogCallbackFn _setLogCallback;
  late final _GetVersionFn _getVersion;
  late final _DestroyFn _destroy;
  late final _GetLastErrorFn _getLastError;

  HevSocks5Bindings() {
    // Загружаем библиотеку
    if (Platform.isAndroid) {
      _lib = ffi.DynamicLibrary.open('libhev_socks5.so');
    } else if (Platform.isIOS || Platform.isMacOS) {
      _lib = ffi.DynamicLibrary.process();
    } else if (Platform.isLinux) {
      _lib = ffi.DynamicLibrary.open('libhev_socks5.so');
    } else if (Platform.isWindows) {
      _lib = ffi.DynamicLibrary.open('hev_socks5.dll');
    } else {
      throw UnsupportedError('Unsupported platform');
    }

    // Привязываем функции
    _create = _lib
        .lookupFunction<
          HevSocks5Instance Function(),
          HevSocks5Instance Function()
        >('hev_socks5_create');
    _init = _lib
        .lookupFunction<
          ffi.Bool Function(
            HevSocks5Instance,
            ffi.Pointer<NativeHevSocks5Config>,
          ),
          bool Function(HevSocks5Instance, ffi.Pointer<NativeHevSocks5Config>)
        >('hev_socks5_init');
    _start = _lib
        .lookupFunction<
          ffi.Bool Function(HevSocks5Instance),
          bool Function(HevSocks5Instance)
        >('hev_socks5_start');
    _stop = _lib
        .lookupFunction<
          ffi.Void Function(HevSocks5Instance),
          void Function(HevSocks5Instance)
        >('hev_socks5_stop');
    _isRunning = _lib
        .lookupFunction<
          ffi.Bool Function(HevSocks5Instance),
          bool Function(HevSocks5Instance)
        >('hev_socks5_is_running');
    _getStats = _lib
        .lookupFunction<
          ffi.Bool Function(
            HevSocks5Instance,
            ffi.Pointer<NativeHevSocks5Stats>,
          ),
          bool Function(HevSocks5Instance, ffi.Pointer<NativeHevSocks5Stats>)
        >('hev_socks5_get_stats');
    _setLogCallback = _lib
        .lookupFunction<
          ffi.Void Function(
            HevSocks5Instance,
            ffi.Pointer<ffi.NativeFunction<NativeLogCallback>>,
            ffi.Pointer<ffi.Void>,
          ),
          void Function(
            HevSocks5Instance,
            ffi.Pointer<ffi.NativeFunction<NativeLogCallback>>,
            ffi.Pointer<ffi.Void>,
          )
        >('hev_socks5_set_log_callback');
    _getVersion = _lib
        .lookupFunction<
          ffi.Pointer<Utf8> Function(),
          ffi.Pointer<Utf8> Function()
        >('hev_socks5_get_version');
    _destroy = _lib
        .lookupFunction<
          ffi.Void Function(HevSocks5Instance),
          void Function(HevSocks5Instance)
        >('hev_socks5_destroy');
    _getLastError = _lib
        .lookupFunction<
          ffi.Pointer<Utf8> Function(HevSocks5Instance),
          ffi.Pointer<Utf8> Function(HevSocks5Instance)
        >('hev_socks5_get_last_error');
  }

  HevSocks5Instance create() => _create();
  bool init(
    HevSocks5Instance instance,
    ffi.Pointer<NativeHevSocks5Config> config,
  ) => _init(instance, config);
  bool start(HevSocks5Instance instance) => _start(instance);
  void stop(HevSocks5Instance instance) => _stop(instance);
  bool isRunning(HevSocks5Instance instance) => _isRunning(instance);
  bool getStats(
    HevSocks5Instance instance,
    ffi.Pointer<NativeHevSocks5Stats> stats,
  ) => _getStats(instance, stats);
  void setLogCallback(
    HevSocks5Instance instance,
    ffi.Pointer<ffi.NativeFunction<NativeLogCallback>> callback,
    ffi.Pointer<ffi.Void> userData,
  ) => _setLogCallback(instance, callback, userData);
  String getVersion() => _getVersion().toDartString();
  void destroy(HevSocks5Instance instance) => _destroy(instance);
  String getLastError(HevSocks5Instance instance) =>
      _getLastError(instance).toDartString();
}

typedef _CreateFn = HevSocks5Instance Function();
typedef _InitFn =
    bool Function(HevSocks5Instance, ffi.Pointer<NativeHevSocks5Config>);
typedef _StartFn = bool Function(HevSocks5Instance);
typedef _StopFn = void Function(HevSocks5Instance);
typedef _IsRunningFn = bool Function(HevSocks5Instance);
typedef _GetStatsFn =
    bool Function(HevSocks5Instance, ffi.Pointer<NativeHevSocks5Stats>);
typedef _SetLogCallbackFn =
    void Function(
      HevSocks5Instance,
      ffi.Pointer<ffi.NativeFunction<NativeLogCallback>>,
      ffi.Pointer<ffi.Void>,
    );
typedef _GetVersionFn = ffi.Pointer<Utf8> Function();
typedef _DestroyFn = void Function(HevSocks5Instance);
typedef _GetLastErrorFn = ffi.Pointer<Utf8> Function(HevSocks5Instance);

/// High-level Dart API for HevSocks5
class HevSocks5 {
  late final HevSocks5Bindings _bindings;
  HevSocks5Instance? _instance;

  /// Log callback
  void Function(String level, String message)? onLog;

  HevSocks5() {
    _bindings = HevSocks5Bindings();
  }

  /// Get library version
  String get version => _bindings.getVersion();

  /// Initialize with configuration
  bool initialize(HevSocks5Config config) {
    // Создаем экземпляр
    _instance = _bindings.create();
    if (_instance == null || _instance == ffi.nullptr) {
      return false;
    }

    // Создаем нативную конфигурацию
    final nativeConfig = calloc<NativeHevSocks5Config>();
    nativeConfig.ref.socks5_server = config.socks5Server.toNativeUtf8();
    nativeConfig.ref.tun_name = config.tunName.toNativeUtf8();
    nativeConfig.ref.tun_address = config.tunAddress.toNativeUtf8();
    nativeConfig.ref.tun_gateway = config.tunGateway.toNativeUtf8();
    nativeConfig.ref.tun_netmask = config.tunNetmask.toNativeUtf8();
    nativeConfig.ref.dns_server = config.dnsServer.toNativeUtf8();
    nativeConfig.ref.tun_mtu = config.tunMtu;
    nativeConfig.ref.tun_fd = config.tunFd;
    nativeConfig.ref.enable_ipv6 = config.enableIpv6;
    nativeConfig.ref.username = config.username?.toNativeUtf8() ?? ffi.nullptr;
    nativeConfig.ref.password = config.password?.toNativeUtf8() ?? ffi.nullptr;

    // Инициализируем
    final result = _bindings.init(_instance!, nativeConfig);

    // Освобождаем память
    calloc.free(nativeConfig.ref.socks5_server);
    calloc.free(nativeConfig.ref.tun_name);
    calloc.free(nativeConfig.ref.tun_address);
    calloc.free(nativeConfig.ref.tun_gateway);
    calloc.free(nativeConfig.ref.tun_netmask);
    calloc.free(nativeConfig.ref.dns_server);
    if (nativeConfig.ref.username != ffi.nullptr)
      calloc.free(nativeConfig.ref.username);
    if (nativeConfig.ref.password != ffi.nullptr)
      calloc.free(nativeConfig.ref.password);
    calloc.free(nativeConfig);

    return result;
  }

  /// Start the tunnel
  bool start() {
    if (_instance == null) return false;
    return _bindings.start(_instance!);
  }

  /// Stop the tunnel
  void stop() {
    if (_instance == null) return;
    _bindings.stop(_instance!);
  }

  /// Check if running
  bool get isRunning {
    if (_instance == null) return false;
    return _bindings.isRunning(_instance!);
  }

  /// Get statistics
  HevSocks5Stats? getStats() {
    if (_instance == null) return null;

    final nativeStats = calloc<NativeHevSocks5Stats>();
    final result = _bindings.getStats(_instance!, nativeStats);

    if (!result) {
      calloc.free(nativeStats);
      return null;
    }

    final stats = HevSocks5Stats(
      txPackets: nativeStats.ref.tx_packets,
      rxPackets: nativeStats.ref.rx_packets,
      txBytes: nativeStats.ref.tx_bytes,
      rxBytes: nativeStats.ref.rx_bytes,
    );

    calloc.free(nativeStats);
    return stats;
  }

  /// Get last error
  String? getLastError() {
    if (_instance == null) return null;
    return _bindings.getLastError(_instance!);
  }

  /// Dispose resources
  void dispose() {
    if (_instance != null) {
      stop();
      _bindings.destroy(_instance!);
      _instance = null;
    }
  }
}
