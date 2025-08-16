import { AddressBar, App } from "./component";
import "./style/index.css";

const app = App.new();
let address_bar = AddressBar.new();
app.addElement(address_bar);

export type payload = {
    init: { url: string };
    test: { one: number; two: number };
};

declare global {
    interface HTMLElementEventMap {
        initialize: CustomEvent<payload["init"]>;
    }
}

window.chrome.webview.addEventListener<payload>("message", (event) => {
    switch (event.data.type) {
        case "init":
            {
                // if (address_bar) {
                //     address_bar.dispatchEvent(
                //         new CustomEvent("initialize", { detail: event.data.payload }),
                //     );
                // }
            }
            break;
        case "test":
            {
                // console.log(event.data);
            }
            break;
    }
});

// function testMessages() {
//     const init: WebViewMessageEventData<payload> = {
//         type: "init",
//         payload: {
//             name: "Cindy Thompson",
//             age: 18,
//         },
//     };

//     window.chrome.webview.postMessage(init);

//     const test: WebViewMessageEventData<payload> = {
//         type: "test",
//         payload: {
//             one: 23,
//             two: 162,
//         },
//     };

//     window.chrome.webview.postMessage(test);
// }
