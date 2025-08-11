import "./styles/index.css";
import "./webview";

window.chrome.webview.addEventListener("message", (arg) => {
    console.log(arg.data);
});
