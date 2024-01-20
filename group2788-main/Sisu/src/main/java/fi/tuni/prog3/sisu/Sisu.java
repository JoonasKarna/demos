package fi.tuni.prog3.sisu;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;
import static javafx.application.Application.launch;

/**
 * JavaFX App
 */
public class Sisu extends Application {

    private static Scene scene;
    /**
     * sets the stage and shows it
     * @param stage
     * @throws IOException 
     */
    @Override
    public void start(Stage stage) throws IOException {
        scene = new Scene(loadFXML("student"), 870, 900);
        scene.getStylesheets().add("Stylesheet.css");
        stage.setScene(scene);
        stage.setTitle("Sisusta Selviää");
        stage.show();
    }
    /**
     * sets the main root FXML file that is loaded
     * @param fxml
     * @throws IOException 
     */
    static void setRoot(String fxml) throws IOException {
        scene.setRoot(loadFXML(fxml));
    }
    /**
     * 
     * @param fxml
     * @return
     * @throws IOException 
     */
    private static Parent loadFXML(String fxml) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(Sisu.class.getResource(fxml + ".fxml"));
        return fxmlLoader.load();
    }
    /**
     * Main function, creation of student object
     * @param args 
     */
    public static void main(String[] args) {
        launch();
        
    }

}