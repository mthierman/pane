import "./index.css";

import type { AddressBar } from "./address_bar";
import type { Button } from "./button";
import type { payload } from "./payload";
import { getElementById } from "./utility";

const element = {
    button: getElementById<Button>("button"),
    address_bar: getElementById<AddressBar>("address-bar"),
};

window.chrome.webview.addEventListener<payload>("message", (event) => {
    switch (event.data.type) {
        case "init":
            {
                console.log(event.data);

                if (element.button) {
                    element.button.innerText = event.data.payload.name;
                }

                if (element.address_bar) {
                    element.address_bar.value = event.data.payload.name;
                }
            }
            break;
        case "test":
            {
                console.log(event.data);
            }
            break;
    }
});
