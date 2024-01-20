
package fi.tuni.prog3.sisu;

import java.io.IOException;
import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;

/**
 *  Students login controller makes the user put correct student number
 *  in order to change or watch students courses and degrees etc
 *  @author Joonas Kärnä
 */

public class StudentController {
    //Creation of student for passing information
    private Student studentObj = new Student();
    //class that information is passed with
    StudentHolder studentData = StudentHolder.getInstance();   
    
    //input field for student number
    @FXML
    private TextField studentNumberField;
    //Label that informs login errors to the user
    @FXML
    private Label errorLabel;
    /**
     * logs the student and opens secondary window
     * @throws IOException 
     */
    @FXML
    private void studentLogin() throws IOException, Exception {
        
        try {
            //getting id from FXML textField
            int studentNumber = Integer.parseInt(studentNumberField.getText());
        
            System.out.println("Student number "+studentNumber);
            //we read the student basic information from the file
            studentObj.readFromFile("test.json");
            System.out.println(studentObj.getName());
            
            //if student number exists we can move the user to next page
            if(studentNumber == studentObj.getStudentNumber())
            {
                studentData.setStudent(studentObj);
                
                Sisu.setRoot("courses");
                
            }
            else
            {
                //if student number is not found we inform the user
                errorLabel.setText("No such student number found");
            }
            //we also inform the user if input field is empty and the user
            //tries to login without any student number
        }catch (Exception e)
        {
            errorLabel.setText("Fill in the student number");
            System.out.println(e);
        }
    }
    /**
     * exits the program
     * @throws IOException 
     */
    @FXML
    private void exitProgram() throws IOException {
        Platform.exit();
    }
}
