/**
 * ConfirmBox
 * generické okno, kterému se při zavolání předá jaký má mít název a text, nasledne se potvrdi ano ne
 * author: Vojtěch Bůbela <xbubel08>
 */

import javafx.stage.*;
import javafx.scene.*;
import javafx.scene.layout.*;
import javafx.scene.control.*;
import javafx.geometry.*;

/**
 *  ConfirmBox je volán pokud je potřeba potvrzení akce od uživatele
 */
public class ConfirmBox {
    
    static boolean answer;

    public static boolean display(String title, String message) {
        Stage window = new Stage();

        window.initModality(Modality.APPLICATION_MODAL);
        window.setTitle(title);
        window.setMinWidth(200);

        Label label = new Label();
        label.setText(message);

        Button yesButton = new Button("yes");
        Button noButton = new Button("no");

        yesButton.setOnAction(e -> {
            window.close();
            answer = true;
        });

        noButton.setOnAction(e -> {
            window.close();
            answer = false;
        });

        VBox layout = new VBox(10);
        layout.getChildren().addAll(label, yesButton, noButton);
        layout.setAlignment(Pos.CENTER);

        Scene scene = new Scene(layout, 300, 250);
        window.setScene(scene);
        window.showAndWait();

        return answer;
    }
}
