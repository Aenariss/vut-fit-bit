/**
 * PopupBox
 * generické okno, kterému se při zavolání předá jaký má mít název a text
 * author: Vojtěch Bůbela <xbubel08>
 */

package src.GUI.boxes;

import javafx.stage.*;
import javafx.scene.*;
import javafx.scene.layout.*;
import javafx.scene.control.*;
import javafx.geometry.*;

/**
 *  PopupBox je volán pokaždé když je potřeba jednoduché okno s textovou zprávou
 */
public class PopupBox {
        

    public static void display(String title, String message) {
        Stage window = new Stage();

        window.initModality(Modality.APPLICATION_MODAL);
        window.setTitle(title);
        window.setMinWidth(350);

        Label label = new Label();
        label.setText(message);
        label.setStyle("-fx-font: 14 arial;");
        Button closeButton = new Button("Close the window");
        closeButton.setOnAction(e -> window.close());

        BorderPane.setAlignment(closeButton, Pos.CENTER);
        BorderPane root = new BorderPane();
        root.setPadding(new Insets(10, 10, 10, 10));

        root.setPrefSize(200,150);
        root.setCenter(label);
        root.setBottom(closeButton);
        
        Scene scene = new Scene(root);
        window.setScene(scene);
        window.showAndWait();
        
    }
}

