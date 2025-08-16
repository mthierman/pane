export class App {
    html = document.documentElement as HTMLHtmlElement;
    body = document.body as HTMLBodyElement;

    static new() {
        return new App();
    }

    static addElement<T extends Node>(element: T) {
        const body = document.body as HTMLBodyElement;
        return body.appendChild(element);
    }
}
