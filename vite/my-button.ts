import "./styles/index.css";

// SHADOW DOM:

// class MyButton extends HTMLElement {
//     constructor() {
//         super();
//         const shadow = this.attachShadow({ mode: "open" });
//         const button = document.createElement("button");
//         button.textContent = "Click me";
//         button.onclick = () => {
//             console.log("Button clicked!");
//         };
//         shadow.appendChild(button);
//     }
// }

// customElements.define("my-button", MyButton);

// NO SHADOW DOM

// class MyButton extends HTMLElement {
//     connectedCallback() {
//         this.innerHTML = `<button>Click me</button>`;
//         this.querySelector("button")!.onclick = () => console.log("Button clicked!");
//     }
// }

// customElements.define("my-button", MyButton);

// NO SHADOW DOM, NO TEMPLATE LITERALS

class MyButton extends HTMLElement {
    connectedCallback() {
        const button = document.createElement("button");
        // button.classList.add("bg-red-800");
        button.textContent = "Click me";
        button.onclick = () => console.log("Button clicked!");
        this.appendChild(button);
    }
}

customElements.define("my-button", MyButton);
