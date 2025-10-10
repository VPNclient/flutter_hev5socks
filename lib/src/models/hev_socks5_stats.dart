/// Statistics for HevSocks5 tunnel
class HevSocks5Stats {
  /// Transmitted packets
  final int txPackets;

  /// Received packets
  final int rxPackets;

  /// Transmitted bytes
  final int txBytes;

  /// Received bytes
  final int rxBytes;

  const HevSocks5Stats({
    this.txPackets = 0,
    this.rxPackets = 0,
    this.txBytes = 0,
    this.rxBytes = 0,
  });

  /// Total packets
  int get totalPackets => txPackets + rxPackets;

  /// Total bytes
  int get totalBytes => txBytes + rxBytes;

  /// Formatted transmitted bytes
  String get formattedTxBytes => _formatBytes(txBytes);

  /// Formatted received bytes
  String get formattedRxBytes => _formatBytes(rxBytes);

  /// Formatted total bytes
  String get formattedTotalBytes => _formatBytes(totalBytes);

  static String _formatBytes(int bytes) {
    if (bytes < 1024) return '$bytes B';
    if (bytes < 1024 * 1024) return '${(bytes / 1024).toStringAsFixed(2)} KB';
    if (bytes < 1024 * 1024 * 1024) {
      return '${(bytes / (1024 * 1024)).toStringAsFixed(2)} MB';
    }
    return '${(bytes / (1024 * 1024 * 1024)).toStringAsFixed(2)} GB';
  }

  factory HevSocks5Stats.fromMap(Map<String, dynamic> map) {
    return HevSocks5Stats(
      txPackets: map['txPackets'] as int? ?? 0,
      rxPackets: map['rxPackets'] as int? ?? 0,
      txBytes: map['txBytes'] as int? ?? 0,
      rxBytes: map['rxBytes'] as int? ?? 0,
    );
  }

  Map<String, dynamic> toMap() {
    return {
      'txPackets': txPackets,
      'rxPackets': rxPackets,
      'txBytes': txBytes,
      'rxBytes': rxBytes,
    };
  }

  @override
  String toString() {
    return 'HevSocks5Stats(TX: $formattedTxBytes, RX: $formattedRxBytes, '
        'Packets: $totalPackets)';
  }
}
