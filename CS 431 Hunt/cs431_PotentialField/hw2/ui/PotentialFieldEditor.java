package hw2.ui;
/*
 * PotentialFieldEditor.java
 *
 * Created on February 15, 2005, 12:56 PM
 */

/**
 *
 * @author Kenny Hunt
 */
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import javax.swing.*;

import hw2.models.PotentialField;

public class PotentialFieldEditor extends JPanel implements ItemListener {
    private PotentialField field;
    private JComboBox typeBox;
    private JTextField dropoffField, cutoffField;
    private static String[] types = {"Attractive", "Repulsive", "Tangential", "Uniform"};
    
    public void itemStateChanged(ItemEvent ev) {
        
    }
    
    public class TangentialPanel extends JPanel implements ActionListener {
        JRadioButton cwButton, ccwButton;
        
        public void actionPerformed(ActionEvent ev) {
            
        }
        
        TangentialPanel() {
            ButtonGroup group = new ButtonGroup();
            
            cwButton = new JRadioButton("CW");
            cwButton.addActionListener(this);
            ccwButton = new JRadioButton("CCW");
            ccwButton.addActionListener(this);
            
            group.add(cwButton);
            group.add(ccwButton);
            
            add(cwButton);
            add(ccwButton);
            
        }
    }
    
    /** Creates a new instance of PotentialFieldEditor */
    public PotentialFieldEditor(PotentialField field) {
        this.field = field;
        
        setLayout(new GridLayout(4,2));
        
        typeBox = new JComboBox(types);
        typeBox.addItemListener(this);
        add(new JLabel("Type"));
        add(typeBox);
        
        dropoffField = new JTextField("");
        add(new JLabel("Mag Profile"));
        add(dropoffField);
        
        cutoffField = new JTextField("");
        add(new JLabel("Cutoff Distance"));
        add(cutoffField);
        
    }
    
}
