// import { App } from "./app";
import type { ComponentEventMap } from "./app";
import { AddressBar } from "./component";
import "./style/index.css";

let address_bar = AddressBar.new();

export type payload = {
    init: { url: string };
    test: { one: number; two: number };
};

declare global {
    interface HTMLElementEventMap extends ComponentEventMap<payload> {}
}

window.chrome.webview.addEventListener<payload>("message", (event) => {
    switch (event.data.type) {
        case "init":
            {
                if (address_bar) {
                    address_bar.dispatchEvent(
                        new CustomEvent("init", { detail: event.data.payload }),
                    );
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
