/**
 * Třída sloužící k zapnutí GUI a inicializaci jeho prvků.
 *
 * @author Vojtěch Bůbela <xbubel08>
 */

import javafx.application.Application;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.StackPane;
import javafx.stage.WindowEvent;
import javafx.stage.Stage;
// import jdk.javadoc.internal.tool.Start;
import javafx.scene.Parent;
import javafx.fxml.FXMLLoader;
import javafx.scene.layout.VBox;

import src.shelf_manipulation.store.*;
import src.shelf_manipulation.goods.*;
import src.cart.*;
import src.utils.*;
import src.map_manipulation.MapControl;

import java.time.LocalDate;
import java.util.concurrent.TimeUnit;
import java.util.Arrays;


/**
 * Třída sloužící ke spuštění GUI.
 */
public class Main extends Application {

    
   
    /**
    * main funkce, která spustí GUI.
    * @param args Vstupní argumenty programu
    */
    public static void main(String[] args) {

        launch(args);

    }

    /**
    * Start funkce připraví GUI.
    */
    @Override
    public void start(Stage primaryStage) throws Exception {

        FXMLLoader myloader = new FXMLLoader(getClass().getResource("/layout.fxml"));

        VBox root = myloader.load();



        
        MapControl map = new MapControl();

        Scene scene = new Scene(root);
        primaryStage.setScene(scene);
        primaryStage.setTitle("Sklad");
        primaryStage.show();
        
        

        main_controller controller = myloader.getController();


        controller.main_loop_prepare();

        primaryStage.setOnCloseRequest(new EventHandler<WindowEvent>() {
            
        /**
        * handle slouží ke korektnímu ukončení programu po kliknutí na křížek.
        */
        @Override
        public void handle(WindowEvent e) {
            
            Platform.exit();
            System.exit(0);
            }
        });
        
    }
}
