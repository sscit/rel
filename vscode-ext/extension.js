const { LanguageClient } = require('vscode-languageclient')

module.exports = {
  activate(context) {
    const executable = {
      command: './rel_ls',
      args: [''],
    }

    const serverOptions = {
      run: executable,
      debug: executable,
    }

    const clientOptions = {
      documentSelector: [{
        scheme: 'file',
        language: 'plaintext',
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