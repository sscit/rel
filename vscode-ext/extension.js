const { LanguageClient } = require('vscode-languageclient')

module.exports = {
  activate(context) {
    const executable = {
      command: './rel_ls',
      args: ['-vv'],
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