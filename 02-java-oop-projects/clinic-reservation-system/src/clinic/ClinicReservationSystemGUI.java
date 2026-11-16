package clinic;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.util.List;
import java.text.*;

public class ClinicReservationSystemGUI {
    private CRS crs;
    private JFrame frame;
    private JTextArea textArea;
    private JPanel panel;
    private JButton addHospitalButton, addSectionButton, addDoctorButton, addPatientButton, addRendezvousButton, viewRendezvousButton;
    private JLabel statusLabel;

    public ClinicReservationSystemGUI(CRS crs) {
        this.crs = crs;
        frame = new JFrame("Clinic Reservation System");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(800, 600);
        frame.setLocationRelativeTo(null);

        textArea = new JTextArea(5, 30);
        textArea.setEditable(false);
        textArea.setLineWrap(true);
        textArea.setWrapStyleWord(true);
        JScrollPane scrollPane = new JScrollPane(textArea);

        panel = new JPanel();
        panel.setLayout(new GridBagLayout());
        panel.setBackground(new Color(240, 240, 240));

        addHospitalButton = createStyledButton("Add Hospital");
        addSectionButton = createStyledButton("Add Section");
        addDoctorButton = createStyledButton("Add Doctor");
        addPatientButton = createStyledButton("Add Patient");
        addRendezvousButton = createStyledButton("Add Rendezvous");
        viewRendezvousButton = createStyledButton("View Rendezvous");

        setButtonStyles();

        statusLabel = new JLabel("");
        statusLabel.setForeground(new Color(204, 0, 0));
        statusLabel.setFont(new Font("Arial", Font.PLAIN, 14));

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(10, 10, 10, 10);
        gbc.fill = GridBagConstraints.HORIZONTAL;

        addButtonsToPanel(gbc);

        addSectionButton.setEnabled(false);
        addDoctorButton.setEnabled(false);
        addPatientButton.setEnabled(false);
        addRendezvousButton.setEnabled(false);

        addButtonActions();

        frame.add(panel);
        frame.setVisible(true);
    }

    private JButton createStyledButton(String text) {
        JButton button = new JButton(text);
        button.setPreferredSize(new Dimension(200, 50));
        button.setBackground(new Color(255, 87, 34));
        button.setForeground(Color.WHITE);
        button.setFont(new Font("Arial", Font.BOLD, 14));
        button.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        button.setFocusPainted(false);
        button.setOpaque(true);
        button.setContentAreaFilled(true);
        return button;
    }

    private void setButtonStyles() {
        addHospitalButton.setBorder(BorderFactory.createLineBorder(new Color(255, 87, 34), 2, true));
        addSectionButton.setBorder(BorderFactory.createLineBorder(new Color(255, 87, 34), 2, true));
        addDoctorButton.setBorder(BorderFactory.createLineBorder(new Color(255, 87, 34), 2, true));
        addPatientButton.setBorder(BorderFactory.createLineBorder(new Color(255, 87, 34), 2, true));
        addRendezvousButton.setBorder(BorderFactory.createLineBorder(new Color(255, 87, 34), 2, true));
        viewRendezvousButton.setBorder(BorderFactory.createLineBorder(new Color(255, 87, 34), 2, true));

        addHospitalButton.addMouseListener(new ButtonHoverEffect(addHospitalButton));
        addSectionButton.addMouseListener(new ButtonHoverEffect(addSectionButton));
        addDoctorButton.addMouseListener(new ButtonHoverEffect(addDoctorButton));
        addPatientButton.addMouseListener(new ButtonHoverEffect(addPatientButton));
        addRendezvousButton.addMouseListener(new ButtonHoverEffect(addRendezvousButton));
        viewRendezvousButton.addMouseListener(new ButtonHoverEffect(viewRendezvousButton));
    }

    private void addButtonsToPanel(GridBagConstraints gbc) {
        gbc.gridx = 0;
        gbc.gridy = 0;
        panel.add(addHospitalButton, gbc);

        gbc.gridx = 1;
        gbc.gridy = 0;
        panel.add(addSectionButton, gbc);

        gbc.gridx = 0;
        gbc.gridy = 1;
        panel.add(addDoctorButton, gbc);

        gbc.gridx = 1;
        gbc.gridy = 1;
        panel.add(addPatientButton, gbc);

        gbc.gridx = 0;
        gbc.gridy = 2;
        panel.add(addRendezvousButton, gbc);

        gbc.gridx = 1;
        gbc.gridy = 2;
        panel.add(viewRendezvousButton, gbc);

        gbc.gridx = 0;
        gbc.gridy = 3;
        gbc.gridwidth = 2;
        panel.add(statusLabel, gbc);

        gbc.gridx = 0;
        gbc.gridy = 4;
        gbc.gridwidth = 2;
        panel.add(new JScrollPane(textArea), gbc);
    }

    private void addButtonActions() {
        addHospitalButton.addActionListener(e -> addHospital());
        addSectionButton.addActionListener(e -> addSection());
        addDoctorButton.addActionListener(e -> addDoctor());
        addPatientButton.addActionListener(e -> addPatient());
        addRendezvousButton.addActionListener(e -> addRendezvous());
        viewRendezvousButton.addActionListener(e -> viewRendezvous());
    }

    private void addHospital() {
        try {
            String hospitalId = JOptionPane.showInputDialog("Enter Hospital ID:");
            String hospitalName = JOptionPane.showInputDialog("Enter Hospital Name:");

            if (hospitalExists(hospitalId, hospitalName)) {
                JOptionPane.showMessageDialog(frame, "This hospital ID or name already exists.", "Duplicate Hospital", JOptionPane.ERROR_MESSAGE);
            } else {
                crs.addHospital(hospitalId, hospitalName, "");
                textArea.append("Hospital added: " + hospitalName + "\n");
                addSectionButton.setEnabled(true);
            }
        } catch (IllegalArgumentException ex) {
            JOptionPane.showMessageDialog(frame, ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
        } catch (Exception ex) {
            statusLabel.setText("Error adding hospital.");
            ex.printStackTrace();
        }
    }
    
    private void addSection() {
        try {
            String hospitalId = JOptionPane.showInputDialog("Enter Hospital ID:");
            String sectionId = JOptionPane.showInputDialog("Enter Section ID:");
            String sectionName = JOptionPane.showInputDialog("Enter Section Name:");

            if (!hospitalExists(hospitalId, sectionName)) {
                JOptionPane.showMessageDialog(frame, "Invalid Hospital ID. Please enter a valid hospital ID.", "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            crs.addSection(hospitalId, sectionId, sectionName);
            textArea.append("Section added: " + sectionName + "\n");

            addDoctorButton.setEnabled(true);
        } catch (Exception ex) {
            statusLabel.setText("Error adding section.");
            ex.printStackTrace();
        }
    }

    private boolean hospitalExists(String hospitalId, String hospitalName) {
        for (Hospital hospital : crs.getHospitals()) {
            if (hospital.getHospitalId().equals(hospitalId) || hospital.getHospitalName().equalsIgnoreCase(hospitalName)) {
                return true;
            }
        }
        return false;
    }

    private void addDoctor() {
        try {
            String hospitalId = JOptionPane.showInputDialog("Enter Hospital ID:");
            String sectionId = JOptionPane.showInputDialog("Enter Section ID:");
            String doctorName = JOptionPane.showInputDialog("Enter Doctor Name:");
            String doctorId = JOptionPane.showInputDialog("Enter Doctor ID:");
            String doctorDiplomaId = JOptionPane.showInputDialog("Enter Doctor Diploma ID:");

            if (doctorExists(doctorId)) {
                JOptionPane.showMessageDialog(frame, "This doctor ID already exists.", "Duplicate Doctor", JOptionPane.ERROR_MESSAGE);
            } else {
                crs.addDoctor(hospitalId, sectionId, doctorId, doctorName, doctorDiplomaId);
                textArea.append("Doctor added: " + doctorName + "\n");
                addPatientButton.setEnabled(true);
            }
        } catch (IllegalArgumentException ex) {
            JOptionPane.showMessageDialog(frame, ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
        } catch (Exception ex) {
            statusLabel.setText("Error adding doctor.");
            ex.printStackTrace();
        }
    }

    private boolean doctorExists(String doctorId) {
        for (Doctor doctor : crs.getDoctors()) {
            if (doctor.getDoctorId().equals(doctorId)) {
                return true;
            }
        }
        return false;
    }

    private void addPatient() {
        try {
            String patientName = JOptionPane.showInputDialog("Enter Patient Name:");
            String patientId = JOptionPane.showInputDialog("Enter Patient ID:");

            if (patientExists(patientId)) {
                JOptionPane.showMessageDialog(frame, "This patient ID already exists.", "Duplicate Patient", JOptionPane.ERROR_MESSAGE);
            } else {
                crs.addPatient(patientId, patientName);
                textArea.append("Patient added: " + patientName + "\n");
                addRendezvousButton.setEnabled(true);
            }
        } catch (IllegalArgumentException ex) {
            JOptionPane.showMessageDialog(frame, ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
        } catch (Exception ex) {
            statusLabel.setText("Error adding patient.");
            ex.printStackTrace();
        }
    }

    private boolean patientExists(String patientId) {
        for (Patient patient : crs.getPatients()) {
            if (patient.getPatientId().equals(patientId)) {
                return true;
            }
        }
        return false;
    }

    private void addRendezvous() {
        try {
            String hospitalId = JOptionPane.showInputDialog("Enter Hospital ID:");
            String sectionId = JOptionPane.showInputDialog("Enter Section ID:");
            String doctorId = JOptionPane.showInputDialog("Enter Doctor ID:");
            String patientId = JOptionPane.showInputDialog("Enter Patient ID:");
            String dateString = JOptionPane.showInputDialog("Enter Appointment Date (YYYY-MM-DD):");

            if (hospitalId != null && sectionId != null && doctorId != null && patientId != null && dateString != null) {
                try {
                    Date date = new SimpleDateFormat("yyyy-MM-dd").parse(dateString);
                    crs.addAppointment(hospitalId, sectionId, doctorId, patientId, date);
                    textArea.append("Rendezvous added for: " + dateString + "\n");
                } catch (ParseException ex) {
                    statusLabel.setText("Invalid date format.");
                    ex.printStackTrace();
                }
            }
        } catch (Exception ex) {
            statusLabel.setText("Error adding rendezvous.");
            ex.printStackTrace();
        }
    }

    private void viewRendezvous() {
        JFrame rendezvousFrame = new JFrame("View Rendezvous");
        rendezvousFrame.setSize(600, 400);
        rendezvousFrame.setLocationRelativeTo(frame);

        JTextArea rendezvousArea = new JTextArea();
        rendezvousArea.setEditable(false);
        rendezvousArea.setLineWrap(true);
        rendezvousArea.setWrapStyleWord(true);

        java.util.List<Appointment> appointments = crs.getAppointments();
        for (Appointment appointment : appointments) {
            rendezvousArea.append("Hospital: " + appointment.getHospitalId() +
                    ", Section: " + appointment.getSectionId() +
                    ", Doctor ID: " + appointment.getDoctorId() +
                    ", Patient ID: " + appointment.getPatientId() +
                    ", Date: " + appointment.getDate() + "\n");
        }

        rendezvousFrame.add(new JScrollPane(rendezvousArea));
        rendezvousFrame.setVisible(true);
    }

    public static void main(String[] args) {
        CRS crs = new CRS();
        new ClinicReservationSystemGUI(crs);
    }

    private class ButtonHoverEffect extends java.awt.event.MouseAdapter {
        private JButton button;

        public ButtonHoverEffect(JButton button) {
            this.button = button;
        }

        @Override
        public void mouseEntered(MouseEvent e) {
            button.setBackground(new Color(255, 140, 0)); 
        }

        @Override
        public void mouseExited(MouseEvent e) {
            button.setBackground(new Color(255, 87, 34)); 
        }
    }
}
