import { AddressBar } from "./component/address_bar";
import { Button } from "./component/button";

export class Component {
    static register() {
        Button.define();
        AddressBar.define();
    }
}

export { AddressBar, Button };
