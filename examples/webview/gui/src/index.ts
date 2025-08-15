import "./index.css";

import type { AddressBar } from "./address_bar";
import type { Button } from "./button";
import { getElementById } from "./utility";

const button = getElementById<Button>("button");
const address_bar = getElementById<AddressBar>("address-bar");

export type payload = {
    init: { name: string; age: number };
    test: { one: number; two: number };
};

window.chrome.webview.addEventListener<payload>("message", (event) => {
    switch (event.data.type) {
        case "init":
            {
                console.log(event.data);

                if (button) {
                    button.innerText = event.data.payload.name;
                }

                if (address_bar) {
                    address_bar.value = event.data.payload.name;
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
