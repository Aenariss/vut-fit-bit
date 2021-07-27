/**
* main_controller
* main_controller spravuje akce co se stanou v gui
* @author Vojtěch Bubela <xbubel08>
* @author Vojtěch Fiala <xfiala61>
*/

import javafx.fxml.FXML;
import javafx.scene.input.ScrollEvent;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.GridPane;
import javafx.scene.shape.Rectangle;
import javafx.stage.Popup;
import javafx.geometry.HPos;
import javafx.geometry.VPos;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.effect.DisplacementMap;
import javafx.scene.Node;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.GridPane;
import javafx.collections.ObservableList;
import javafx.scene.control.MenuItem;

import src.shelf_manipulation.store.*;
import src.shelf_manipulation.goods.*;
import src.cart.*;
import src.utils.*;
import src.map_manipulation.MapControl;
import src.GUI.boxes.*;

import java.time.LocalDate;
import java.time.LocalTime;
import java.util.concurrent.TimeUnit;
import java.util.Arrays;
import java.util.Timer;
import java.util.TimerTask;
import java.util.HashMap;
import java.util.ArrayList;

/**
 * Třída sloužící ke správě objektů v GUI.
 */
public class main_controller {

    private int loop_started = 0;
    private double timer_speed = 1000;

    @FXML
    private AnchorPane main_content;

    @FXML
    private Button slow_button;

    @FXML
    private Button speed_button;

    @FXML
    private Button default_speed;

    @FXML
    private GridPane main_grid;

    @FXML
    private Button start_button;

    @FXML
    private TextField textbox;

    @FXML 
    private Button get_item_button;

    @FXML
    private MenuItem menu_quit;

    @FXML
    private MenuItem help_menu;

    @FXML
    private TextField free_forklifts;

    @FXML
    private TextField pending_orders;

    @FXML
    private TextField cart_status;

    private Timer timer;
    private LocalTime time = LocalTime.now();

    // start of the main
    private WarehouseLoader loader;
    private WaitList wait_list;
    private CoordsConverter cnv = new CoordsConverter();
    private HashIndexFinder fnd = new HashIndexFinder();
    private MapControl map = new MapControl();
    private RandomNumberGenerator rng = new RandomNumberGenerator();

    private CartControl cart;
    private CartControl cart2;
    private CartControl cart3;
    private CartControl cart4;
    private CartControl cart5;

    private int cart_flag = 0;
    private int cart_flag2 = 0;
    private int cart_flag3 = 0;
    private int cart_flag4 = 0;
    private int cart_flag5 = 0;

    private String[] end_points = {"0.37", "0.38", "0.39"};
    private int rand_min = 0;
    private int rand_max = end_points.length;   // Tady by logicky melo byt -1, ale tohle funguje lip

    private int[] pos;

    private ShelfManipulator regal;

    private Image vydej = new Image("/vydejni_misto.png");
    private Image start = new Image("/parking_slot.png");
    private Image forklift = new Image("/forklift.png");
    private Image ground = new Image("/ground.png");
    private Image block = new Image("/block.png");
    private Image trail = new Image("/trail.png");

    private ObservableList<Node> childrens;

    @FXML
    private void display_help(ActionEvent event) {

        String help = "Aplikace se ovláda pomocí bočního panelu\na interaktivní hlavní plochy\nV mape skladu se dá kliknout na:\nvozík, regál nebo zem";
        PopupBox.display("Nápověda", help);
    }

    @FXML
    private void menu_quit_program(ActionEvent event) {
        Platform.exit();
        System.exit(0);
    }

    @FXML
    private void get_item() {
            
        String str = textbox.getText();
        boolean flag = false;

        // List l = loader.goods_list;
        for (String item : loader.goods_list) {
            if (item.equals(str)) {
                flag = true;
                break;
            }
        }
        if (flag) {
            wait_list.WaitList_Add(str);
        }
        else {
            PopupBox.display("Dovez", "Zboží neexistuje, zkuste to znovu");
        }
    }

    @FXML 
    private void start_gui(ActionEvent event) {
        if(loop_started == 0) {
            count_time();
            loop_started = 1;
        }
    }

    @FXML 
    private void restart_gui(ActionEvent event) {
        main_loop_prepare();
        count_time();
        loop_started = 1;
    }

    @FXML
    private void slow_down(ActionEvent event) {
        if(timer_speed <= 4000) {
            timer.cancel();
            timer_speed = 1.6 * timer_speed;
            count_time();
        }
    }

    @FXML
    private void speed_up(ActionEvent event) {
        if(timer_speed >= 250) {
            timer.cancel();
            timer_speed = 0.625 * timer_speed;
            count_time();
        }
    }

    @FXML
    private void set_default_speed(ActionEvent event) {
        
        timer.cancel();
        timer_speed = 1000;
        count_time();
    }

    @FXML
    private void on_zoom(ScrollEvent event) {
        event.consume();
        double zoom_level = event.getDeltaY();

        if(zoom_level == 0) {
            zoom_level = 1;
        } else if(zoom_level < 0){
            zoom_level = 0.9;
        } else {
            zoom_level = 1.1;
        }

        main_content.setScaleX(zoom_level *main_content.getScaleX());
        main_content.setScaleY(zoom_level *main_content.getScaleY());
    }

    private void cart_does_stuff(CartControl cart) {
        if (cart.cart_go4ware == true) {
            if (cart.cart_busy == true) {
                cart.cart_busy = false; // Jedno kolo cekani (nakladani)
                cart.cart_loaded = true; // Je nalozeno, cesta do odberu
                cart.cart_go4ware = false; // Uz nejede pro zbozi
                cart.content.loadItem();
                cart.content.setShelf(null);

                int res = rng.getRandomNumber(rand_min, rand_max);
                int[] exit_point = cnv.coordsInt(end_points[res]);

                cart.current_path = cart.findPath(exit_point[0], exit_point[1]);    // cesta do cile
            }
            else {
                pos = cnv.coordsInt(cart.current_path[cart.current_path.length-2]); // nova pozice je predposledni souradnice ze soucastne cesty
                cart.gotoPosition(pos[0], pos[1]);  // Jdi na tu pozici (tzn. mam treba [0,1  1,1  2,1], kde 2,1 je aktualni -> Jdi na 1,1)
                int[] target_refresh = cnv.coordsInt(cart.current_path[0]); // Nacti cil z puvodni cesty
                cart.current_path = cart.findPath(target_refresh[0], target_refresh[1]);    // Spocitej novou cestu z nove polohy do puvodniho cile

                // Takhle se to chova, kdyz to dojede do cile -> Zustavaji v ceste 2 body
                if (cart.current_path.length > 1) {
                // Takze je porovnej, jestli jsou totozne a jestli jo, je vozik v cili a ukonci cyklus
                    if (cart.current_path[0].equals(cart.current_path[1])) {
                        if (cart.cart_busy != true) {
                            cart.cart_busy = true;
                        }
                    }
                }
            }
        }
        else if (cart.cart_loaded == true) {
            if (cart.cart_busy == true) {
                cart.cart_busy = false;
                cart.cart_loaded = false;   // vylozeno do vydeje
                cart.cart_start = true;
                cart.content.unloadItem();

                int[] start = cart.getOriginalStart(); // Docasny hardcode cile (mozna random vybrat jeden z moznych?) (maybe permanent hardcode)
                cart.current_path = cart.findPath(start[0], start[1]);    // cesta na start
            }
            else {
                pos = cnv.coordsInt(cart.current_path[cart.current_path.length-2]); // nova pozice je predposledni souradnice ze soucastne cesty
                cart.gotoPosition(pos[0], pos[1]);  // Jdi na tu pozici (tzn. mam treba [0,1  1,1  2,1], kde 2,1 je aktualni -> Jdi na 1,1)
                int[] target_refresh = cnv.coordsInt(cart.current_path[0]); // Nacti cil z puvodni cesty
                cart.current_path = cart.findPath(target_refresh[0], target_refresh[1]);    // Spocitej novou cestu z nove polohy do puvodniho cile

                // Takhle se to chova, kdyz to dojede do cile -> Zustavaji v ceste 2 body
                if (cart.current_path.length > 1) {
                    // Takze je porovnej, jestli jsou totozne a jestli jo, je vozik v cili a ukonci cyklus
                     if (cart.current_path[0].equals(cart.current_path[1])) {
                        if (cart.cart_busy != true) {
                            cart.cart_busy = true;
                        }
                    }
                }
            }
        }
        else if (cart.cart_start == true) {
            pos = cnv.coordsInt(cart.current_path[cart.current_path.length-2]); // nova pozice je predposledni souradnice ze soucastne cesty
            cart.gotoPosition(pos[0], pos[1]);  // Jdi na tu pozici (tzn. mam treba [0,1  1,1  2,1], kde 2,1 je aktualni -> Jdi na 1,1)
            int[] target_refresh = cnv.coordsInt(cart.current_path[0]); // Nacti cil z puvodni cesty
            cart.current_path = cart.findPath(target_refresh[0], target_refresh[1]);    // Spocitej novou cestu z nove polohy do puvodniho cile

            // Takhle se to chova, kdyz to dojede do cile -> Zustavaji v ceste 2 body
            if (cart.current_path.length > 1) {
                // Takze je porovnej, jestli jsou totozne a jestli jo, je vozik v cili a ukonci cyklus
                if (cart.current_path[0].equals(cart.current_path[1])) {
                    cart.cart_start = false;
                    cart.free_carts.add(cart);
                    cart.current_path = null;
                }
            }
        }
    }

    private void onClick(MouseEvent event) {

        int column = GridPane.getColumnIndex((Node) event.getSource());
        int row = GridPane.getRowIndex((Node) event.getSource());

        int result = this.map.getItem(row, column);

        if(result == 5) {
            String str = cnv.convertCoords(row, column);

            if(regal.shelf_map.get(str) == null) {
                PopupBox.display("regal", "regál je prazdný");
                return;
            } 
            
            String goods_type = regal.shelf_map.get(str).getShelfType();
            int polozky = regal.shelf_map.get(str).size();
            
            String mssg = goods_type + " : " + polozky;
            
            PopupBox.display("regál", mssg);
            
        } else if (result == 7) {
          
            if(cart_flag == 1 || cart_flag2 == 1|| cart_flag3 == 1 || cart_flag4 == 1|| cart_flag5 == 1) {
                cart_flag = 0;
                cart_flag2 = 0;
                cart_flag3 = 0;
                cart_flag4 = 0;
                cart_flag5 = 0;

                return;
            }


            if(cart.getPosition()[0] == row && cart.getPosition()[1] == column) {
                if(cart.content.getWare() != null) {
                    cart_flag = 1;
                    PopupBox.display("vozík", cart.content.getWare().goods().getName());
                } else {
                    cart_flag = 1;
                    PopupBox.display("vozík", "Vozík je prázdný");
                }
            } else if(cart2.getPosition()[0] == row && cart2.getPosition()[1] == column) {
                if(cart2.content.getWare() != null) {
                    cart_flag2 = 1;
                    PopupBox.display("vozík", cart2.content.getWare().goods().getName());
                } else {
                    cart_flag2 = 1;
                    PopupBox.display("vozík", "Vozík je prázdný");
                }
            } else if(cart3.getPosition()[0] == row && cart3.getPosition()[1] == column) {
                if(cart3.content.getWare() != null) {
                    cart_flag3 = 1;
                    PopupBox.display("vozík", cart3.content.getWare().goods().getName());
                } else {
                    cart_flag3 = 1;
                    PopupBox.display("vozík", "Vozík je prázdný");
                }
            } else if(cart4.getPosition()[0] == row && cart4.getPosition()[1] == column) {
                if(cart4.content.getWare() != null) {
                    cart_flag4 = 1;
                    PopupBox.display("vozík", cart4.content.getWare().goods().getName());
                } else {
                    cart_flag4 = 1;
                    PopupBox.display("vozík", "Vozík je prázdný");
                }
            } else if(cart5.getPosition()[0] == row && cart5.getPosition()[1] == column) {
                if(cart5.content.getWare() != null) {
                    cart_flag5 = 1;
                    PopupBox.display("vozík", cart5.content.getWare().goods().getName());
                } else {
                    cart_flag5 = 1;
                    PopupBox.display("vozík", "Vozík je prázdný");
                }
            }

        } else if (result == 1) {
            map.insertObstacle(row, column);
            ImageView new_image = new ImageView(this.block);
            new_image.setFitHeight(50);
            new_image.setFitWidth(30);
            main_grid.add(new_image ,column,row);
        } else if (result == 3) {
            map.removeObstacle(row, column);
            ImageView new_image = new ImageView(this.ground);
            new_image.setFitHeight(50);
            new_image.setFitWidth(30);
            main_grid.add(new_image ,column,row);
        }
    }

    /**
    * Funkce sloužící k aktualizaci mapy danou rychlostí.
    */
    public void count_time() {
        timer = new Timer(false);
        timer.scheduleAtFixedRate(new TimerTask(){
            @Override
            public void run() {
                // Pokud bylo zadano, ze se ma nejake zbozi dovest
                if (wait_list.WaitList_Len() > 0) {
                    // A zaroven je volny vozik
                    if (cart.free_carts.size() > 0) {
                        // Tak vyber prvni ze seznamu volnych voziku, ktery tohle zbozi odveze
                        CartControl active = cart.free_carts.get(0);
                        cart.free_carts.remove(active); // odstran ho ze seznamu volnych
                        String dest = fnd.getDestination(regal.shelf_map, wait_list.WaitList[0]); // Ziskej souradnice, kam ma vozik dojet
                        String regal_position = fnd.getIndex(regal.shelf_map, wait_list.WaitList[0]);
                        int[] destination = cnv.coordsInt(dest);    // zkonvertuj je
                        active.current_path = active.findPath(destination[0], destination[1]);  // Spocitej pocatecni cestu
                        wait_list.WaitList_Remove_First();  // Smaz z wait_listu zbozi, co vozik zpracovava
                        active.cart_go4ware = true;
                        active.content.setShelf(regal.shelf_map.get(regal_position));
                    }
                }

                // Tady bude nejspis funkce, ktera tohle zahrne a nebo 5x tentyz kod :)
                // Zkontroluj, jestli ma vozik vubec nekam jet a nebo je volny
                if (cart.current_path != null) {
                    // Zkontroluj, ze cesta je delsi nez 0
                    if (cart.current_path.length > 0) {
                        cart_does_stuff(cart);
                    }
                }

                if (cart2.current_path != null) {
                    // Zkontroluj, ze cesta je delsi nez 0
                    if (cart2.current_path.length > 0) {
                        cart_does_stuff(cart2);
                    }
                }

                if (cart3.current_path != null) {
                    // Zkontroluj, ze cesta je delsi nez 0
                    if (cart3.current_path.length > 0) {
                        cart_does_stuff(cart3);
                    }
                }

                if (cart4.current_path != null) {
                    // Zkontroluj, ze cesta je delsi nez 0
                    if (cart4.current_path.length > 0) {
                        cart_does_stuff(cart4);
                    }
                }

                if (cart5.current_path != null) {
                    // Zkontroluj, ze cesta je delsi nez 0
                    if (cart5.current_path.length > 0) {
                        cart_does_stuff(cart5);
                    }
                }

                int[][] orig_map = map.getOriginalMap();
                int[][] new_map = new int[orig_map.length][orig_map[0].length];
                int[][] curr_map = map.getMap();
                int[] cart1_pos = cart.getPosition();
                int[] cart2_pos = cart2.getPosition();
                int[] cart3_pos = cart3.getPosition();
                int[] cart4_pos = cart4.getPosition();
                int[] cart5_pos = cart5.getPosition();

                for (int i = 0; i < orig_map.length; i++) {
                    for (int k = 0; k < orig_map[0].length; k++) {
                        if ((cart1_pos[0] == i && cart1_pos[1] == k) || (cart2_pos[0] == i && cart2_pos[1] == k) ||
                        (cart3_pos[0] == i && cart3_pos[1] == k) || (cart4_pos[0] == i && cart4_pos[1] == k) || (cart5_pos[0] == i && cart5_pos[1] == k))
                            new_map[i][k] = 7;
                        else if (curr_map[i][k] == 3)
                            new_map[i][k] = 3;
                        else 
                            new_map[i][k] = orig_map[i][k];
                    }
                }

                map.mapSet(new_map);

                int size = 0;
                String str = null;

                size = cart.free_carts.size();


                if (size == 5) {
                    cart_status.setText("Vsechno bylo vyrizene");
                }
                else {
                    cart_status.setText("Objednavky se vyrizuji");
                }

                str = String.valueOf(size);

                free_forklifts.setText(str);

                size = wait_list.WaitList_Len();

                str = String.valueOf(size);

                pending_orders.setText(str);



                Platform.runLater(() -> {
                    main_grid.getChildren().clear();
                    init_gui(map.getMap());
                });

            }
        }, 0,(int) timer_speed);
    }

    /**
    * Funkce sloužící k zobrazení správné mapy skladu.
    * @param map Mapa, na základě které se zobrazí sklad.
    */
    public void init_gui(int[][] map) {

        int row;
        int col;
        int pole[];

        for(int i = 0; i < map.length; i++) {
            for (int k = 0; k < map[0].length; k++) {

                int match_found = 0;

                if(cart_flag == 1) {
                    if(cart.current_path != null) {
                    
                        for(int j = 0; j < cart.current_path.length; j++) {
                            pole = cnv.coordsInt(cart.current_path[j]);
                            row = pole[0];
                            col = pole[1];

                            if(row == i && col == k) {
                                match_found = 1;
                                break;
                            }
                        }
                    }
                }

                if(cart_flag2 == 1) {
                    if(cart2.current_path != null) {
                    
                        for(int j = 0; j < cart2.current_path.length; j++) {
                            pole = cnv.coordsInt(cart2.current_path[j]);
                            row = pole[0];
                            col = pole[1];

                            if(row == i && col == k) {
                                match_found = 1;
                                break;
                            }
                        }
                    }
                }

                if(cart_flag3 == 1) {
                    if(cart.current_path != null) {
                    
                        for(int j = 0; j < cart3.current_path.length; j++) {
                            pole = cnv.coordsInt(cart3.current_path[j]);
                            row = pole[0];
                            col = pole[1];

                            if(row == i && col == k) {
                                match_found = 1;
                                break;
                            }
                        }
                    }
                }


                if(cart_flag4 == 1) {
                    if(cart4.current_path != null) {
                    
                        for(int j = 0; j < cart4.current_path.length; j++) {
                            pole = cnv.coordsInt(cart4.current_path[j]);
                            row = pole[0];
                            col = pole[1];

                            if(row == i && col == k) {
                                match_found = 1;
                                break;
                            }
                        }
                    }
                }

                if(cart_flag5 == 1) {
                    if(cart5.current_path != null) {
                    
                        for(int j = 0; j < cart5.current_path.length; j++) {
                            pole = cnv.coordsInt(cart5.current_path[j]);
                            row = pole[0];
                            col = pole[1];

                            if(row == i && col == k) {
                                match_found = 1;
                                break;
                            }
                        }
                    }
                }


                if(map[i][k] == 5) {
                    Rectangle rec = new Rectangle(30,50);
                    main_grid.add(rec ,k,i);
                    GridPane.setHalignment(rec, HPos.CENTER);
                    GridPane.setValignment(rec, VPos.CENTER);
                } else if(map[i][k] == 9) {
                    // obrazek je prevzaty z:
                    // https://www.pinterest.com/pin/342203271663134343/
                    ImageView new_image = new ImageView(vydej);
                    new_image.setFitHeight(50);
                    new_image.setFitWidth(30);
                    main_grid.add(new_image ,k,i);
                } else if (map[i][k] == 0) {
                    // Rectangle rec = new Rectangle(30,50);
                    ImageView new_image = new ImageView(start);
                    new_image.setFitHeight(50);
                    new_image.setFitWidth(30);
                    main_grid.add(new_image ,k,i);
                } else if (map[i][k] == 7) {
                    // Rectangle rec = new Rectangle(30,50);
                    ImageView new_image = new ImageView(forklift);
                    new_image.setFitHeight(50);
                    new_image.setFitWidth(30);
                    main_grid.add(new_image ,k,i);
                } else if (map[i][k] == 1) {
                    if(match_found == 1) {
                        ImageView new_image = new ImageView(this.trail);
                        new_image.setFitHeight(50);
                        new_image.setFitWidth(30);
                        main_grid.add(new_image ,k,i);
                    } else {
                        ImageView new_image = new ImageView(this.ground);
                        new_image.setFitHeight(50);
                        new_image.setFitWidth(30);
                        main_grid.add(new_image ,k,i);
                    }

                } else if (map[i][k] == 3) {
                    ImageView new_image = new ImageView(this.block);
                    new_image.setFitHeight(50);
                    new_image.setFitWidth(30);
                    main_grid.add(new_image ,k,i);
                }

            }
        }

        Node result = null;
        childrens = main_grid.getChildren();

        for (Node node : childrens) {
            node.setOnMouseClicked(this::onClick);
        }
    }

    /**
    * Funkce k inicializaci skladu.
    * Používá se i v rámci případného restartu skladu.
    */
    public void main_loop_prepare() {
        // vynuluj waitlist
        wait_list.WaitList = new String[0];
        wait_list = new WaitList();

        int[][] map_orig = map.getOriginalMap();
        int[][] nm = new int[map_orig.length][map_orig[0].length];

        for (int i = 0; i < map_orig.length; i++) {
            for (int k = 0; k < map_orig[0].length; k++) {
                nm[i][k] = map_orig[i][k];
            }
        }
        if (timer != null) {
            timer.cancel();
            timer_speed = 1000;
        }
        // vyresetuj flagy
        cart_flag = 0;
        cart_flag2 = 0;
        cart_flag3 = 0;
        cart_flag4 = 0;
        cart_flag5 = 0;

        map.mapSet(nm);

        // vyresetuj volne voziky
        if (cart.free_carts != null)
            cart.free_carts = new ArrayList<CartControl>();
        
        // Vytvoreni voziku
        if (cart != null) {
            cart = null;
            cart2 = null;
            cart3 = null;
            cart4 = null;
            cart5 = null; 
        }
        cart = new CartControl("11.38");
        cart2 = new CartControl("11.37");
        cart3 = new CartControl("11.36");
        cart4 = new CartControl("10.38");
        cart5 = new CartControl("10.37");


        init_gui(nm);

        if (regal != null) {
            regal.shelf_map = new HashMap<String, GoodsShelf>();
            regal = null;
        }

        // vynuluj seznam zbozi
        loader.goods_list = new String[0];
        loader = null;
        loader = new WarehouseLoader(); // naplneni skladu

        // Kontrolni regal, ktery je pouzit k pristupu k ostatnim
        regal = loader.controller;

        free_forklifts.setText(String.valueOf(cart.free_carts.size()));
        cart_status.setText("Probiha inicializace...");
        

        int size = wait_list.WaitList_Len();

        String str = String.valueOf(size);

        pending_orders.setText(str);


    }
}   