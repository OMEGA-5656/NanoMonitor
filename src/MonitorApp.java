import javax.swing.*;
import java.awt.*;

public class MonitorApp extends JFrame {
    private final JLabel cpuLabel;
    private final JLabel ramLabel;
    private final JLabel statusLabel;

    static {
        System.loadLibrary("stats_bridge");
    }

    public native String getStats();

    public MonitorApp() {
        setTitle("NanoMonitor");
        setSize(360, 180);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setLayout(new GridLayout(4, 1, 8, 8));

        cpuLabel = new JLabel("CPU: --%", SwingConstants.CENTER);
        ramLabel = new JLabel("RAM: -- / -- GB", SwingConstants.CENTER);
        statusLabel = new JLabel("Status: Ready", SwingConstants.CENTER);
        JButton refreshButton = new JButton("Refresh");

        refreshButton.addActionListener(e -> refreshStats());

        add(cpuLabel);
        add(ramLabel);
        add(refreshButton);
        add(statusLabel);
    }

    private void refreshStats() {
        try {
            String result = getStats();
            if (result.startsWith("Error:")) {
                statusLabel.setText("Status: " + result);
                return;
            }

            String[] parts = result.split("\\|", 2);
            if (parts.length == 2) {
                cpuLabel.setText(parts[0].trim());
                ramLabel.setText(parts[1].trim());
                statusLabel.setText("Status: Updated");
            } else {
                statusLabel.setText("Status: Error: bad response");
            }
        } catch (UnsatisfiedLinkError ex) {
            statusLabel.setText("Status: Error: native library not loaded");
        } catch (Exception ex) {
            statusLabel.setText("Status: Error: " + ex.getMessage());
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            MonitorApp app = new MonitorApp();
            app.setVisible(true);
            app.refreshStats();
        });
    }
}
