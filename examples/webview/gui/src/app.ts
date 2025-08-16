export const App = {
    html: document.documentElement as HTMLHtmlElement,
    body: document.body as HTMLBodyElement,

    addElement<T extends Node>(element: T) {
        const body = document.body as HTMLBodyElement;
        return body.appendChild(element);
    },
};
