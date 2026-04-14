import javax.swing.*;
import java.awt.*;

public class MonitorApp extends JFrame {
    private final JButton button = new JButton("Click to Load Stats...");
    private final Timer autoRefreshTimer;

    static { System.loadLibrary("stats_bridge"); }
    public native String getStats();

    public MonitorApp() {
        setTitle("Mini Monitor");
        setSize(420, 180);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLayout(new GridLayout(1, 1));

        button.addActionListener(e -> refreshStats());

        add(button);

        // Refresh every second to keep values up-to-date.
        autoRefreshTimer = new Timer(1000, e -> refreshStats());
        autoRefreshTimer.start();

        setVisible(true);
        refreshStats();
    }

    private void refreshStats() {
        try {
            button.setText(getStats());
        } catch (UnsatisfiedLinkError ex) {
            button.setText("Error: native library not loaded");
        } catch (Exception ex) {
            button.setText("Error: " + ex.getMessage());
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(MonitorApp::new);
    }
}