This file will look really ugly if you have it open in vim, I'd recommend opening it in GitHub.

<ol> 
    <li>Make an SSH Key
        <ol>
            <li>Open "Git Bash"</li>
            <li>Change to the ~/.ssh directory (cd ~/.ssh)</li>
            <li>Generate an SSH key by typing ssh-keygen</li>
                <ol>
                    <li>Hit "enter" any time it asks you a question (default RSA type, blank passphrase)</li>
                </ol>
            <li>Display the contents of the new public key (cat id_rsa.pub)</li>
            <li>Copy all the contents it displayed (right click to copy in GitBash, ctrl+c won't work)</li>
        </ol>
    </li>
    <li>Add the SSH key to your github account
        <ol>
            <li>In your web browser, go to the github website.</li>
            <li>Open your account Settings on the github website (click the user icon in upper-right corner of the web page and choose "settings")</li>
            <li>Select "SSH and GPG Keys" on the left</li>
            <li>Click "New SSH key" green button</li>
            <li>Give it a name like "232 ssh key"</li>
            <li>Paste the key you copied into the large text box</li>
            <li>Click "Add SSH key" button</li>
        </ol>
    </li>
    <li>Use the key to clone your repo
        <ol>
            <li>In your web browser, open your new repository in the github web page (the URL it gave you when you first created it)</li>
            <li>Click the green "Code" button</li>
            <li>Note that it says "Clone with SSH": copy that URL (looks like git@github.com/....)</li>
            <li>Go back to Git Bash and git clone that copied URL</li>
        </ol>
    </li>
</ol>
