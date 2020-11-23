const { LanguageClient } = require('vscode-languageclient')

module.exports = {
  activate(context) {
    const executable = {
      command: '$HOME/bin/rel-ls/rel_ls',
      args: ['-vv'],
      options: {
        env: process.env,
        shell: true,
      }
    }

    const serverOptions = {
      run: executable,
      debug: executable,
    }

    const clientOptions = {
      documentSelector: [{
        scheme: 'file',
        language: 'rel',
      }],
    }

    const client = new LanguageClient(
      'rel-extension-id',
      'RELLanguageClient',
      serverOptions,
      clientOptions
    )

    context.subscriptions.push(client.start())
  },
}