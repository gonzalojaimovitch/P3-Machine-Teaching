#!/usr/bin/env python3
#Adapted for machine teaching experiments
import fire
import json
import os
import numpy as np
import tensorflow as tf

from src import model, sample, encoder

def nointeract_model(
    model_name,
    seed,
    nsamples,
    batch_size,
    length,
    temperature,
    top_k,
    top_p,
    models_dir,
    inputGPT2, 
    nrepeat, 
    filePath,
    nconcepts,
    nphases,
    ntest
):
    """
    Non Interactively run the model
    :model_name=124M : String, which model to use
    :seed=None : Integer seed for random number generators, fix seed to reproduce
     results
    :nsamples=1 : Number of samples to return total
    :batch_size=1 : Number of batches (only affects speed/memory).  Must divide nsamples.
    :length=None : Number of tokens in generated text, if None (default), is
     determined by model hyperparameters
    :temperature=1 : Float value controlling randomness in boltzmann
     distribution. Lower temperature results in less random completions. As the
     temperature approaches zero, the model will become deterministic and
     repetitive. Higher temperature results in more random completions.
    :top_k=0 : Integer value controlling diversity. 1 means only 1 word is
     considered for each step (token), resulting in deterministic completions,
     while 40 means 40 words are considered at each step. 0 (default) is a
     special setting meaning no restrictions. 40 generally is a good value.
     :models_dir : path to parent folder containing model subfolders
     (i.e. contains the <model_name> folder)
    """
    models_dir = os.path.expanduser(os.path.expandvars(models_dir))
    if batch_size is None:
        batch_size = 1
    assert nsamples % batch_size == 0

    enc = encoder.get_encoder(model_name, models_dir)
    hparams = model.default_hparams()
    with open(os.path.join(models_dir, model_name, 'hparams.json')) as f:
        hparams.override_from_dict(json.load(f))

    if length is None:
        length = hparams.n_ctx // 2
    elif length > hparams.n_ctx:
        raise ValueError("Can't get samples longer than window size: %s" % hparams.n_ctx)

    with tf.Session(graph=tf.Graph()) as sess:
        context = tf.placeholder(tf.int32, [batch_size, None])
        np.random.seed(seed)
        tf.set_random_seed(seed)
        output = sample.sample_sequence(
            hparams=hparams, length=length,
            context=context,
            batch_size=batch_size,
            temperature=temperature, top_k=top_k, top_p=top_p
        )

        saver = tf.train.Saver()
        ckpt = tf.train.latest_checkpoint(os.path.join(models_dir, model_name))
        saver.restore(sess, ckpt)
		
        f = open(filePath, 'w')

        for c in range(1, nconcepts + 1):
          for p in range(1, nphases + 1):
            for t in range(1, ntest + 1):
              pos = (c-1)*nphases*ntest + (t - 1)
              for r in range(1, nrepeat + 1):
                raw_text = inputGPT2[pos]
                context_tokens = enc.encode(raw_text)
                for _ in range(nsamples // batch_size):
                  out = sess.run(output, feed_dict={
                    context: [context_tokens for _ in range(batch_size)]
                  })[:, len(context_tokens):]
                  for i in range(batch_size):
                    text = enc.decode(out[i])
                    f.write("Concept " + str(c) + " Phase " + str(p) + " Test " + str(t) + " Repetition " + str(r) + " // Input= " + raw_text + " // Output= " + text)
                  

def rungpt2(inputGPT2, nrepeat, filePath, nconcepts, nphases, ntest):
	nointeract_model('774M', None, 1, 1, 3, 1, 0, 1, 'models', inputGPT2, nrepeat, filePath, nconcepts, nphases, ntest)
