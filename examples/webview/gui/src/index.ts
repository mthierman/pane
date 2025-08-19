import { Page } from "pane";
import "./component/address_bar";
import "./component/button";
import "./index.css";
import "./reset.css";

declare global {
    interface HTMLElementEventMap extends WebViewMessageEventMap<payload> {}
}

export type payload = {
    init: { url: string };
    test: { one: number; two: number };
};

const app = {
    address_bar: Page.createElement("pane-address-bar"),
};

window.chrome.webview.addEventListener<payload>("message", (event) => {
    switch (event.data.type) {
        case "init":
            {
                app.address_bar.attach();
                app.address_bar.dispatch(event);
            }
            break;
        case "test":
            {
                console.log(event);
            }
            break;
    }
});
