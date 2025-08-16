export class Body {
    body = document.body as HTMLBodyElement | null;

    static new() {
        return new Body();
    }

    addElement(element: Node) {
        if (this.body) {
            this.body.appendChild(element);
        }
    }
}
