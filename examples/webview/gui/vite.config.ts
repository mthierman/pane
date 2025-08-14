import { type CommonServerOptions, type UserConfig, defineConfig } from "vite";

// https://vitejs.dev/config/
export default defineConfig(({ command }) => {
    const userConfig: UserConfig = {
        base: "./",
    };

    const commonServerOptions: CommonServerOptions = {};

    switch (command) {
        case "serve": {
            return {
                ...userConfig,
                server: { ...commonServerOptions },
                preview: { ...commonServerOptions },
            };
        }
        case "build": {
            return {
                ...userConfig,
            };
        }
    }
});
