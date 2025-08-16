import { AddressBar } from "./component";
import { Utility } from "./utility";

import "./style/index.css";

export type payload = {
    init: { url: string };
    test: { one: number; two: number };
};

declare global {
    interface HTMLElementEventMap extends WebViewEvents<payload> {}
}

let address_bar = AddressBar.new();

window.chrome.webview.addEventListener<payload>("message", (event) => {
    switch (event.data.type) {
        case "init":
            {
                address_bar.dispatchCustomEvent(event);
            }
            break;
        case "test":
            {
                // console.log(event.data);
            }
            break;
    }
});
