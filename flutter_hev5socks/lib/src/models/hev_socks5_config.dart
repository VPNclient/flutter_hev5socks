/// Configuration for HevSocks5 tunnel
class HevSocks5Config {
  /// SOCKS5 server address (e.g., "127.0.0.1:1080")
  final String socks5Server;

  /// TUN device name
  final String tunName;

  /// TUN IP address
  final String tunAddress;

  /// TUN gateway
  final String tunGateway;

  /// TUN netmask
  final String tunNetmask;

  /// DNS server
  final String dnsServer;

  /// TUN MTU
  final int tunMtu;

  /// TUN file descriptor (-1 for auto)
  final int tunFd;

  /// Enable IPv6
  final bool enableIpv6;

  /// SOCKS5 username (optional)
  final String? username;

  /// SOCKS5 password (optional)
  final String? password;

  const HevSocks5Config({
    required this.socks5Server,
    this.tunName = 'tun0',
    this.tunAddress = '10.0.0.2',
    this.tunGateway = '10.0.0.1',
    this.tunNetmask = '255.255.255.0',
    this.dnsServer = '8.8.8.8',
    this.tunMtu = 1500,
    this.tunFd = -1,
    this.enableIpv6 = false,
    this.username,
    this.password,
  });

  Map<String, dynamic> toMap() {
    return {
      'socks5Server': socks5Server,
      'tunName': tunName,
      'tunAddress': tunAddress,
      'tunGateway': tunGateway,
      'tunNetmask': tunNetmask,
      'dnsServer': dnsServer,
      'tunMtu': tunMtu,
      'tunFd': tunFd,
      'enableIpv6': enableIpv6,
      'username': username,
      'password': password,
    };
  }

  factory HevSocks5Config.fromMap(Map<String, dynamic> map) {
    return HevSocks5Config(
      socks5Server: map['socks5Server'] as String,
      tunName: map['tunName'] as String? ?? 'tun0',
      tunAddress: map['tunAddress'] as String? ?? '10.0.0.2',
      tunGateway: map['tunGateway'] as String? ?? '10.0.0.1',
      tunNetmask: map['tunNetmask'] as String? ?? '255.255.255.0',
      dnsServer: map['dnsServer'] as String? ?? '8.8.8.8',
      tunMtu: map['tunMtu'] as int? ?? 1500,
      tunFd: map['tunFd'] as int? ?? -1,
      enableIpv6: map['enableIpv6'] as bool? ?? false,
      username: map['username'] as String?,
      password: map['password'] as String?,
    );
  }
}




