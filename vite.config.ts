import { resolve } from "path";
import { type CommonServerOptions, type UserConfig, defineConfig } from "vite";
import oxlintPlugin from "vite-plugin-oxlint";

const userConfig: UserConfig = {
    base: "./",
    plugins: [oxlintPlugin()],
};

const commonServerOptions: CommonServerOptions = {
    https: {
        pfx: resolve("../.cert/localhost.pfx"),
        passphrase: "localhost",
    },
};

// https://vitejs.dev/config/
export default defineConfig(({ command }) => {
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
