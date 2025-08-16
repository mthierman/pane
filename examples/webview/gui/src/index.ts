// import { App } from "./app";
import type { ComponentEventMap } from "./app";
import { AddressBar } from "./component";
import { Event } from "./event";
import "./style/index.css";

export type payload = {
    init: { url: string };
    test: { one: number; two: number };
};

declare global {
    interface HTMLElementEventMap extends ComponentEventMap<payload> {}
}

let address_bar = AddressBar.new();

window.chrome.webview.addEventListener<payload>("message", (event) => {
    switch (event.data.type) {
        case "init":
            {
                // if (address_bar) {
                //     address_bar.dispatchEvent(
                //         new CustomEvent(event.data.type, { detail: event.data.payload }),
                //     );
                // }
                if (address_bar) {
                    address_bar.dispatchEvent(event);
                }
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
